#include "pch.h"
#include "nat.h"
#include "cryptopp/randpool.h"
#include "cryptopp/osrng.h"
#include "byte_buffer.h"
#include "transmission.h"
#include "rsa.h"
#include "utc_time.h"

using namespace affix_base;
using affix_base::networking::nat_type;
using namespace CryptoPP;
using namespace affix_base::cryptography;
using namespace asio::ip;
using asio::error_code;
using std::vector;
using affix_base::data::byte_buffer;
using namespace asio;

const size_t RETURNER_RESPONSE_SIZE = 551;
const size_t RETURNER_SEND_SIZE = 25;
const size_t RETURNER_RESPONSE_TIMEOUT = 5;

void networking::socket_reopen_and_rebind(
	udp::socket& a_socket) {

	udp::endpoint l_internal_endpoint = a_socket.local_endpoint();
	a_socket.close();
	a_socket.open(udp::v4());
	a_socket.bind(l_internal_endpoint);

}

bool networking::socket_external_ip(
	udp::socket& a_socket,
	const udp::endpoint& a_returner_endpoint,
	const RSA::PublicKey& a_returner_public_key,
	udp::endpoint& a_output) {

	error_code l_ec;

	vector<byte> l_outbound_data(RETURNER_SEND_SIZE);

	AutoSeededRandomPool random;
	random.GenerateBlock(l_outbound_data.data(), l_outbound_data.size());

	if (!socket_send_data_to(a_socket, a_returner_endpoint, l_outbound_data)) {
		std::cerr << "[ NAT ] Error sending data to returner." << std::endl;
		return false;
	}

	vector<byte> l_inbound_data(RETURNER_RESPONSE_SIZE);

	if (!socket_receive_data_for(a_socket, l_inbound_data, RETURNER_RESPONSE_TIMEOUT)) {
		std::cerr << "[ NAT ] Error receiving data from returner." << std::endl;
		return false;
	}

	byte_buffer response_msg(l_inbound_data);

	vector<byte> l_random_data;
	uint32_t l_external_address;
	uint16_t l_external_port;
	vector<byte> l_signature;

	try {
		if (!response_msg.pop_front(l_random_data)) return false;
		if (!response_msg.pop_front(l_external_address)) return false;
		if (!response_msg.pop_front(l_external_port)) return false;
		if (!response_msg.pop_front(l_signature)) return false;
	}
	catch (...) {
		std::cerr << "[ NAT ] Error unpacking message from returner." << std::endl;
		return false;
	}

	// CHECK THAT THE RANDOM DATA IS THE SAME
	if (!std::equal(l_random_data.begin(), l_random_data.end(), l_outbound_data.begin(), l_outbound_data.end())) {
		std::cerr << "[ NAT ] Mismatched random data." << std::endl;
		return false;
	}

	// REPACK DATA INTO MESSAGE
	byte_buffer l_temp_message;

	if (!l_temp_message.push_back(l_random_data)) return false;
	if (!l_temp_message.push_back(l_external_address)) return false;
	if (!l_temp_message.push_back(l_external_port)) return false;

	vector<byte> l_temp_message_body = l_temp_message.data();

	bool l_signature_valid = false;

	if (!rsa_try_verify(l_temp_message_body, l_signature, a_returner_public_key, l_signature_valid)) {
		std::cerr << "[ NAT ] Error verifying returner signature." << std::endl;
		return false;
	}

	if (!l_signature_valid) {
		std::cerr << "[ NAT ] Invalid signature from returner." << std::endl;
		return false;
	}

	a_output.address(ip::address_v4(l_external_address));
	a_output.port(l_external_port);

	return true;

}

bool networking::socket_external_ip(
	udp::socket& a_socket,
	const udp::endpoint& a_returner_endpoint,
	const RSA::PublicKey& a_returner_public_key,
	udp::endpoint& a_output,
	const size_t& a_max_attempts) {
	
	for (int l_attempt = 0; l_attempt < a_max_attempts; l_attempt++) {
		if (socket_external_ip(a_socket, a_returner_endpoint, a_returner_public_key, a_output)) {
			return true;
		}
	}

	std::cerr << "[ NAT ] Failed after max attempts at gathering external endpoint information from: " << a_returner_endpoint.address().to_string() << ":" << std::to_string(a_returner_endpoint.port()) << std::endl;
	return false;

}

bool networking::socket_nat_type(
	udp::socket& a_socket,
	const udp::endpoint& a_returner_endpoint_0,
	const RSA::PublicKey& a_returner_public_key_0,
	const udp::endpoint& a_returner_endpoint_1,
	const RSA::PublicKey& a_returner_public_key_1,
	const size_t& a_max_attempts,
	nat_type& a_output) {

	// UPON CALLING THIS FUNCTION, IT IS EXPECTED THAT THE SOCKET IS ALREADY OPEN AND BOUND TO A LOCAL ENDPOINT

	udp::endpoint l_external_endpoint_0;
	udp::endpoint l_external_endpoint_1;

	socket_reopen_and_rebind(a_socket);
	if (!socket_external_ip(a_socket, a_returner_endpoint_0, a_returner_public_key_0, l_external_endpoint_0, a_max_attempts)) {
		std::cerr << "[ NAT ] Error receiving endpoint information from returner 0." << std::endl;
		return false;
	}
	socket_reopen_and_rebind(a_socket);
	if (!socket_external_ip(a_socket, a_returner_endpoint_1, a_returner_public_key_1, l_external_endpoint_1, a_max_attempts)) {
		std::cerr << "[ NAT ] Error receiving endpoint information from returner 1." << std::endl;
		return false;
	}

	if (l_external_endpoint_0 == l_external_endpoint_1) {
		a_output = nat_type::non_symmetric;
	}
	else {
		a_output = nat_type::symmetric;
	}

	return true;

}

bool networking::socket_nat_type(
	udp::socket& a_socket, 
	const udp::endpoint& a_returner_endpoint_0, 
	const RSA::PublicKey& a_returner_public_key_0, 
	const udp::endpoint& a_returner_endpoint_1, 
	const RSA::PublicKey& a_returner_public_key_1, 
	const size_t& a_max_attempts, 
	nat_type& a_output, 
	const size_t& a_trials) {
	
	size_t l_nat_type_symmetric_counter = 0;
	size_t l_nat_type_non_symmetric_counter = 0;

	for (int i = 0; i < a_trials; i++) {

		nat_type l_nat_type = nat_type::unknown;

		if (!socket_nat_type(a_socket, a_returner_endpoint_0, a_returner_public_key_0, a_returner_endpoint_1, a_returner_public_key_1, a_max_attempts, l_nat_type)) {
			return false;
		}

		switch (l_nat_type) {
		case nat_type::symmetric:
			l_nat_type_symmetric_counter++;
			break;
		case nat_type::non_symmetric:
			l_nat_type_non_symmetric_counter++;
			break;
		}

	}

	if (l_nat_type_symmetric_counter > l_nat_type_non_symmetric_counter) {
		a_output = nat_type::symmetric;
	}
	else {
		a_output = nat_type::non_symmetric;
	}

	return true;

}

bool networking::socket_internal_ip_address(
	asio::ip::address& a_address
)
{
	try {
		asio::io_service netService;
		udp::resolver   resolver(netService);
		udp::resolver::query query(udp::v4(), "google.com", "");
		udp::resolver::iterator endpoints = resolver.resolve(query);
		udp::endpoint ep = *endpoints;
		udp::socket socket(netService);
		socket.connect(ep);
		asio::ip::address addr = socket.local_endpoint().address();
		a_address = addr;
		return true;
	}
	catch (std::exception& e) {
		std::cerr << "Could not determing local IP Address. Exception: " << e.what() << std::endl;
		return false;
	}
}
