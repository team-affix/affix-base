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

	vector<byte> v(16);

	Integer modulus("0x100000000000000000000000000000000");
	ModularArithmetic ma(modulus);

	Integer i1;
	i1.Decode(v.data(), v.size());
	int i1_byte_count_1 = i1.ByteCount();
	i1 = ma.Add(i1, Integer("0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"));
	i1 = ma.Add(i1, 2);
	int i1_byte_count_2 = i1.ByteCount();
	i1.Encode(v.data(), v.size());

	Integer i2;
	i2.Decode(v.data(), v.size());
	int i2_byte_count_1 = i2.ByteCount();
	i2 = ma.Subtract(i2, 2);
	int i2_byte_count_2 = i2.ByteCount();
	i2.Encode(v.data(), v.size());

	/*vector<byte> l_aes_key = aes_generate_key();
	vector<byte> l_iv = aes_generate_iv_zeroed();

	vector<byte> l_data(10000);
	for (int i = 0; i < l_data.size(); i++)
		l_data[i] = i;

	vector<byte> l_encrypted = aes_encrypt(l_data, l_aes_key, l_iv);
	vector<byte> l_decrypted = aes_decrypt(l_encrypted, l_aes_key, l_iv);

	aes_increment_iv(l_iv);

	l_encrypted = aes_encrypt(l_data, l_aes_key, l_iv);
	l_decrypted = aes_decrypt(l_encrypted, l_aes_key, l_iv);

	aes_increment_iv(l_iv);

	l_encrypted = aes_encrypt(l_data, l_aes_key, l_iv);
	l_decrypted = aes_decrypt(l_encrypted, l_aes_key, l_iv);*/

	return EXIT_SUCCESS;

}