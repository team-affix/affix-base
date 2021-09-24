#include "pch.h"
#include "nat.h"
#include "cryptopp/randpool.h"
#include "cryptopp/osrng.h"
#include "byte_buffer.h"
#include "transmission.h"
#include "rsa.h"
#include "utc_time.h"

#if 1
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

using namespace affix_base;
using affix_base::networking::nat_type;
using namespace CryptoPP;
using namespace affix_base::cryptography;

const size_t RETURNER_RESPONSE_SIZE = 551;
const size_t RETURNER_SEND_SIZE = 25;
const size_t RETURNER_RESPONSE_TIMEOUT = 5;

void networking::socket_reopen_and_rebind(udp::socket& a_socket) {

	udp::endpoint l_internal_endpoint = a_socket.local_endpoint();
	a_socket.close();
	a_socket.open(udp::v4());
	a_socket.bind(l_internal_endpoint);

}

bool networking::socket_external_ip(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint, const RSA::PublicKey& a_returner_public_key, udp::endpoint& a_output) {

	error_code l_ec;

	vector<byte> l_outbound_data(RETURNER_SEND_SIZE);

	AutoSeededRandomPool random;
	random.GenerateBlock(l_outbound_data.data(), l_outbound_data.size());

	if (!socket_send_data_to(a_socket, a_returner_endpoint, l_outbound_data)) {
		LOG("[ NAT ] Error sending data to returner.");
		return false;
	}

	vector<byte> l_inbound_data(RETURNER_RESPONSE_SIZE);

	if (!socket_receive_data_for(a_socket, l_inbound_data, RETURNER_RESPONSE_TIMEOUT)) {
		LOG("[ NAT ] Error receiving data from returner.");
		return false;
	}

	byte_buffer response_msg(l_inbound_data);

	vector<byte> l_random_data;
	uint32_t l_external_address;
	uint16_t l_external_port;
	vector<byte> l_signature;

	try {
		response_msg >> l_random_data >> l_external_address >> l_external_port >> l_signature;
	}
	catch (...) {
		LOG("[ NAT ] Error unpacking message from returner.");
		return false;
	}

	// CHECK THAT THE RANDOM DATA IS THE SAME
	if (!std::equal(l_random_data.begin(), l_random_data.end(), l_outbound_data.begin(), l_outbound_data.end())) {
		LOG("[ NAT ] Mismatched random data.");
		return false;
	}

	// REPACK DATA INTO MESSAGE
	byte_buffer l_temp_message;
	l_temp_message << l_random_data << l_external_address << l_external_port;
	vector<byte> l_temp_message_body = l_temp_message.data();

	bool l_signature_valid = false;

	if (!rsa_try_verify(l_temp_message_body, l_signature, a_returner_public_key, l_signature_valid)) {
		LOG("[ NAT ] Error verifying returner signature.");
		return false;
	}

	if (!l_signature_valid) {
		LOG("[ NAT ] Invalid signature from returner.");
		return false;
	}

	a_output.address(ip::address_v4(l_external_address));
	a_output.port(l_external_port);

	LOG("[ NAT ] Successfully retrieved IP (address, port) pair (from: " << a_returner_endpoint.address().to_string() << ":" << std::to_string(a_returner_endpoint.port()) << "): " << a_output.address().to_string() << ":" << std::to_string(a_output.port()));
	
	return true;
}

bool networking::socket_external_ip(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint, const RSA::PublicKey& a_returner_public_key, udp::endpoint& a_output, const size_t& a_max_attempts) {
	
	for (int l_attempt = 0; l_attempt < a_max_attempts; l_attempt++) {
		if (socket_external_ip(a_socket, a_returner_endpoint, a_returner_public_key, a_output)) {
			return true;
		}
	}

	LOG("[ NAT ] Failed after max attempts at gathering external endpoint information from: " << a_returner_endpoint.address().to_string() << ":" << std::to_string(a_returner_endpoint.port()));
	return false;

}

bool networking::socket_nat_type(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint_0, const RSA::PublicKey& a_returner_public_key_0, const udp::endpoint& a_returner_endpoint_1, const RSA::PublicKey& a_returner_public_key_1, const size_t& a_max_attempts, nat_type& a_output) {

	// UPON CALLING THIS FUNCTION, IT IS EXPECTED THAT THE SOCKET IS ALREADY OPEN AND BOUND TO A LOCAL ENDPOINT

	udp::endpoint l_external_endpoint_0;
	udp::endpoint l_external_endpoint_1;

	socket_reopen_and_rebind(a_socket);
	if (!socket_external_ip(a_socket, a_returner_endpoint_0, a_returner_public_key_0, l_external_endpoint_0, a_max_attempts)) {
		LOG("[ NAT ] Error receiving endpoint information from returner 0.");
		return false;
	}
	socket_reopen_and_rebind(a_socket);
	if (!socket_external_ip(a_socket, a_returner_endpoint_1, a_returner_public_key_1, l_external_endpoint_1, a_max_attempts)) {
		LOG("[ NAT ] Error receiving endpoint information from returner 1.");
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

bool networking::socket_nat_type(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint_0, const RSA::PublicKey& a_returner_public_key_0, const udp::endpoint& a_returner_endpoint_1, const RSA::PublicKey& a_returner_public_key_1, const size_t& a_max_attempts, nat_type& a_output, const size_t& a_trials) {
	
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
