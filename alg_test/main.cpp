#include "affix_base.h"

using namespace CryptoPP;
void write_to_console(vector<byte> v) {
	for (int i = 0; i < v.size(); i++)
		std::cout << std::to_string((uint8_t)v[i]) << ", ";
}

using namespace affix_base::networking;
using namespace affix_base::cryptography;

class server_test {
public:
	io_service l_service;
	udp::socket l_socket;
	vector<byte> v;
	udp::endpoint l_remote_ep;
	server_test(uint16_t a_port) : l_socket(l_service), v(12) {
		l_socket.open(udp::v4());
		l_socket.bind(udp::endpoint(udp::v4(), a_port));
	}
	void server_fn() {
		while (true) {
			l_socket.receive_from(asio::buffer(v.data(), v.size()), l_remote_ep);
			std::cout << "UDP: " << std::to_string(l_remote_ep.port()) << std::endl;
			l_socket.send_to(asio::buffer(v.data(), v.size()), l_remote_ep);
		}
	};

};

class tcp_server_test {
public:
	io_context l_context;
	tcp::acceptor l_acceptor;
	vector<byte> b = vector<byte>(20000);
	tcp_server_test(uint16_t a_port) : l_acceptor(l_context, tcp::endpoint(tcp::v4(), a_port)) {

	}
	void server_fn() {
		while (true) {
			tcp::socket l_socket = l_acceptor.accept();
			std::cout << "TCP: " << std::to_string(l_socket.remote_endpoint().port()) << std::endl;
			l_socket.receive(asio::mutable_buffer(b.data(), b.size()));
		}
	}
};

int main() {

	using namespace affix_base::networking;
	using namespace affix_base::cryptography;

	/*server_test s1(8090);
	server_test s2(8091);
	tcp_server_test s3(8092);

	std::thread thd1([&] { s1.server_fn(); });
	std::thread thd2([&] { s2.server_fn(); });
	std::thread thd3([&] { s3.server_fn(); });

	io_service l_service;
	udp::socket l_socket(l_service);
	udp::endpoint l_endpoint_1(ip::make_address("192.168.1.141"), 8090);
	udp::endpoint l_endpoint_2(ip::make_address("192.168.1.141"), 8091);
	udp::endpoint l_ep_2;
	udp::endpoint l_internal_ep;
	bool valid_ep = false;

	vector<byte> v(12);

	for (int i = 0; i < 12; i++) {
		l_socket.open(udp::v4());
		if (!valid_ep) {
			l_socket.bind(udp::endpoint(udp::v4(), 0));
			l_internal_ep = l_socket.local_endpoint();
			valid_ep = true;
		}
		else {
			l_socket.bind(l_internal_ep);
		}
		if (i % 2 == 0)
			l_socket.send_to(asio::buffer(v.data(), v.size()), l_endpoint_1);
		else
			l_socket.send_to(asio::buffer(v.data(), v.size()), l_endpoint_2);
		l_socket.receive(asio::mutable_buffer(v.data(), v.size()));
		l_socket.close();
	}

	io_context l_context;
	tcp::socket l_tcp_socket(l_context);
	tcp::endpoint l_endpoint_3(ip::make_address("192.168.1.141"), 8092);
	tcp::endpoint l_tcp_internal_ep = tcp::endpoint(l_internal_ep.address(), l_internal_ep.port());
	l_tcp_socket.open(tcp::v4());
	l_tcp_socket.bind(l_tcp_internal_ep);
	l_tcp_socket.connect(l_endpoint_3);
	l_tcp_socket.close();
	
	if (thd1.joinable())
		thd1.join();
	if (thd2.joinable())
		thd2.join();
	if (thd3.joinable())
		thd3.join();*/

	RSA::PublicKey l_returner_public_key_0;
	rsa_import(l_returner_public_key_0, "public0.bin");

	RSA::PublicKey l_returner_public_key_1;
	rsa_import(l_returner_public_key_1, "public1.bin");

	udp::endpoint l_returner_endpoint_0(ip::make_address("192.168.1.141"), 9001);
	udp::endpoint l_returner_endpoint_1(ip::make_address("192.168.1.141"), 9000);

	io_service l_service;

	udp::socket l_socket(l_service);
	l_socket.open(udp::v4());
	l_socket.bind(udp::endpoint(udp::v4(), 0));

	nat_type l_nat_type = nat_type::unknown;

	bool result = socket_nat_type(l_socket, l_returner_endpoint_0, l_returner_public_key_0, l_returner_endpoint_1, l_returner_public_key_1, 1, l_nat_type, 100);

	return EXIT_SUCCESS;

}