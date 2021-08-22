#include "pch.h"
#include "nat.h"
#include "cryptopp/randpool.h"
#include "cryptopp/osrng.h"
#include "message.h"
#include "transmission.h"
#include "rsa.h"

using namespace affix_base;
using affix_base::networking::nat_type;
using namespace CryptoPP;
using namespace affix_base::cryptography;

bool networking::socket_external_ip(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, const RSA::PublicKey& a_returner_public_key, tcp::endpoint& a_output) {

	error_code l_ec;

	if (a_socket.connect(a_returner_endpoint, l_ec))
		return false;

	vector<byte> l_outbound_data(25);

	AutoSeededRandomPool random;
	random.GenerateBlock(l_outbound_data.data(), l_outbound_data.size());

	if (!socket_send_data(a_socket, l_outbound_data)) {
		return false;
	}

	vector<byte> l_inbound_data;

	if (!socket_receive(a_socket, l_inbound_data)) {
		return false;
	}

	message response_msg(l_inbound_data);

	vector<byte> l_random_data;
	uint32_t l_external_address;
	uint16_t l_external_port;
	vector<byte> l_signature;

	try {
		response_msg >> l_random_data >> l_external_address >> l_external_port >> l_signature;
	}
	catch (std::exception ex) {
		LOG("[ ERROR ]" << ex.what());
		return false;
	}

	message l_temp_message;

	l_temp_message << l_random_data << l_external_address << l_external_port;

	bool l_signature_valid = false;

	vector<byte> l_temp_message_body = l_temp_message.serialize();

	if (!rsa_try_verify(l_temp_message_body, l_signature, a_returner_public_key, l_signature_valid)) {
		return false;
	}

	if (!l_signature_valid) {
		return false;
	}

	a_output.address(ip::address_v4(l_external_address));
	a_output.port(l_external_port);
	
	return true;
}

bool networking::socket_nat_type(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, nat_type& a_output) {
	return false;
}
