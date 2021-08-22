#include "affix_base.h"

using namespace CryptoPP;
void write_to_console(vector<byte> v) {
	for (int i = 0; i < v.size(); i++)
		std::cout << std::to_string((uint8_t)v[i]) << ", ";
}

int main() {

	using namespace affix_base::networking;
	using namespace affix_base::cryptography;

	RSA::PublicKey l_returner_pub_key;
	rsa_import(l_returner_pub_key, "public.bin");

	io_context l_context;
	tcp::socket l_socket(l_context);
	tcp::endpoint l_returner_ep(ip::make_address("192.168.1.141"), 8090);
	tcp::endpoint l_external_ep;

	if (!socket_external_ip(l_socket, l_returner_ep, l_returner_pub_key, l_external_ep)) {
		std::cout << "Failure" << std::endl;
	}

	std::cout << l_external_ep.address().to_string() << ":" << std::to_string(l_external_ep.port());

	std::cin.get();

	return EXIT_SUCCESS;

}