#include "affix_base.h"

using namespace CryptoPP;
void write_to_console(vector<byte> v) {
	for (int i = 0; i < v.size(); i++)
		std::cout << std::to_string((uint8_t)v[i]) << ", ";
}

using namespace affix_base::networking;
using namespace affix_base::cryptography;

mutex g_cout_mutex;

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

void returner_test() {

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

	bool result = socket_nat_type(l_socket, l_returner_endpoint_0, l_returner_public_key_0, l_returner_endpoint_1, l_returner_public_key_1, 10, l_nat_type, 1);

}

struct simple_tcp_server {
	ptr<tcp::acceptor> m_acceptor;
	ptr<tcp::socket> m_connection;
	ptr<socket_io_guard> m_socket_io_guard;
	vector<uint8_t> m_data_0;
	vector<uint8_t> m_data_1;
	vector<uint8_t> m_data_2;
	vector<uint8_t> m_data_3;
	vector<uint8_t> m_data_4;
	vector<uint8_t> m_data_5;
};

ptr<simple_tcp_server> prime_tcp_server(io_context& a_context) {

	ptr<simple_tcp_server> result = new simple_tcp_server();
	
	result->m_acceptor = new tcp::acceptor(tcp::acceptor(a_context, tcp::endpoint(tcp::v4(), 8091)));
	result->m_acceptor->async_accept([&, result](error_code a_ec, tcp::socket a_socket) {
		simple_tcp_server& l_result = *result;

		l_result.m_connection = new tcp::socket(std::move(a_socket));
		l_result.m_socket_io_guard = new socket_io_guard(l_result.m_connection.val());

		l_result.m_socket_io_guard->async_receive(l_result.m_data_0, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_0);
			std::cout << std::endl;
		});
		l_result.m_socket_io_guard->async_receive(l_result.m_data_1, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_1);
			std::cout << std::endl;
		});
		l_result.m_socket_io_guard->async_receive(l_result.m_data_2, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_2);
			std::cout << std::endl;
		});
		l_result.m_socket_io_guard->async_receive(l_result.m_data_3, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_3);
			std::cout << std::endl;
		});
		l_result.m_socket_io_guard->async_receive(l_result.m_data_4, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_4);
			std::cout << std::endl;
		});
		l_result.m_socket_io_guard->async_receive(l_result.m_data_5, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_5);
			std::cout << std::endl;
		});

		/*socket_async_receive(l_result.m_connection.val(), l_result.m_data_0, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_0);
			std::cout << std::endl;
		});
		socket_async_receive(l_result.m_connection.val(), l_result.m_data_1, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_1);
			std::cout << std::endl;
		});
		socket_async_receive(l_result.m_connection.val(), l_result.m_data_2, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_2);
			std::cout << std::endl;
		});
		socket_async_receive(l_result.m_connection.val(), l_result.m_data_3, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_3);
			std::cout << std::endl;
		});
		socket_async_receive(l_result.m_connection.val(), l_result.m_data_4, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_4);
			std::cout << std::endl;
		});
		socket_async_receive(l_result.m_connection.val(), l_result.m_data_5, [&, result](bool a_result) {
			lock_guard<mutex> l_lock(g_cout_mutex);
			write_to_console(l_result.m_data_5);
			std::cout << std::endl;
		});*/

	});

	return result;

}

struct sleeper {
	void sleep() {
		for (int i = 0; i < 10; i++) {
			std::cout << "Sleeping " << i << std::endl;
			Sleep(1000);
		}
	}
};

int main() {

	using namespace affix_base::networking;
	using namespace affix_base::cryptography;

	rsa_key_pair kp = rsa_generate_key_pair(2048);

	vector<uint8_t> l_input(10000);
	
	for (int i = 0; i < l_input.size(); i++)
		l_input[i] = i % 56;

	vector<uint8_t> l_encrypted = rsa_encrypt_in_chunks(l_input, kp.public_key);

	vector<uint8_t> l_decrypted = rsa_decrypt_in_chunks(l_encrypted, kp.private_key);

	bool b = std::equal(l_input.begin(), l_input.end(), l_decrypted.begin(), l_decrypted.end());

	return EXIT_SUCCESS;

}