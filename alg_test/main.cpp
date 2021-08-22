#include "affix_base.h"

int main() {

	using namespace affix_base::networking;
	using namespace CryptoPP;

	RSA::PublicKey l_returner_pub_key;
	FileSource fs("public.bin", true);
	l_returner_pub_key.Load(fs);

	io_context l_context;
	tcp::socket l_socket(l_context);
	tcp::endpoint l_returner_ep(ip::make_address("192.168.1.141"), 8090);
	tcp::endpoint l_external_ep;

	if (!socket_external_ip(l_socket, l_returner_ep, l_returner_pub_key, l_external_ep)) {
		std::cout << "Failure" << std::endl;
	}

	return EXIT_SUCCESS;

}