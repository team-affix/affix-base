#include "affix_base.h"
#include <filesystem>

using namespace affix_base::networking;
using namespace affix_base::cryptography;
using std::mutex;
using asio::ip::tcp;
using asio::ip::udp;
using asio::io_service;
using asio::io_context;
using affix_base::data::ptr;
using std::lock_guard;
using std::string;
namespace ip = asio::ip;
using asio::error_code;


using namespace CryptoPP;
void write_to_console(vector<byte> v) {
	for (int i = 0; i < v.size(); i++)
		std::cout << std::to_string((uint8_t)v[i]) << ", ";
}

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

class test_base_class {
public:
	virtual ~test_base_class() {
		std::cout << "deconstructed base" << std::endl;
	}
};

class test_class : public test_base_class {
public:
	virtual ~test_class() {
		std::cout << "deconstructed derived" << std::endl;
	}

public:
	test_class() {

	}

};

struct identity_test {
	string name;

	const string& str() const {
		return name;
	}
};

int my_test_integer = 15;

template<typename T>
class node {
protected:
	vector<ptr<node>> m_inbound_connections;
	vector<ptr<node>> m_outbound_connections;
	T m_val;

protected:
	node() : m_val(T()) {

	}
	node(const T& a_val) : m_val(a_val) {

	}

public:
	static ptr<node> create() {
		return new node();
	}
	static ptr<node> create(const T& a_val) {
		return new node(a_val);
	}

public:
	vector<ptr<node>>& inbound() {
		return m_inbound_connections;
	}
	vector<ptr<node>>& outbound() {
		return m_outbound_connections;
	}
	vector<ptr<node>> startpoints() {
		if (m_inbound_connections.size() == 0) {
			return { this };
		}
		else {
			vector<ptr<node>> result;
			for (int i = 0; i < m_inbound_connections.size(); i++) {
				vector<ptr<node>> l_startpoints = m_inbound_connections[i]->startpoints();
				result.insert(result.end(), l_startpoints.begin(), l_startpoints.end());
			}
			return result;
		}
	}
	vector<ptr<node>> endpoints() {
		if (m_outbound_connections.size() == 0) {
			return { this };
		}
		else {
			vector<ptr<node>> result;
			for (int i = 0; i < m_outbound_connections.size(); i++) {
				vector<ptr<node>> l_endpoints = m_outbound_connections[i]->endpoints();
				result.insert(result.end(), l_endpoints.begin(), l_endpoints.end());
			}
			return result;
		}
	}

public:
	void prepend(ptr<node<T>>& a_other) {
		a_other->m_outbound_connections.push_back(this);
		m_inbound_connections.push_back(a_other);
	}
	void append(ptr<node<T>>& a_other) {
		a_other->m_inbound_connections.push_back(this);
		m_outbound_connections.push_back(a_other);
	}

public:
	T& val() {
		return m_val;
	}

};

struct quality {
	string name;
	string requirements;
};

int main_encrypt_decrypt()
{
	using namespace affix_base::callback;
	using namespace affix_base::threading;
	namespace fs = std::filesystem;

	std::cout << "To encrypt original/->encrypt/, type E" << std::endl;
	std::cout << "    (This will require the folders called \"original\" and \"encrypt\" to exist, and the file public.dat to be in the current directory.)" << std::endl;
	std::cout << "To decrypt encrypt/->decrypt/ type D" << std::endl;
	std::cout << "    (This will require the folders called \"encrypt\" and \"decrypt\" to exist, and the file private.dat to be in the current directory.)" << std::endl;

	std::filesystem::path l_original_path = fs::absolute("original/");
	std::filesystem::path l_encrypt_path = fs::absolute("encrypt/");
	std::filesystem::path l_decrypt_path = fs::absolute("decrypt/");

	fs::path l_input_path;
	fs::path l_output_path;

	auto num_output_files = [&]
	{
		int result = 0;
		for (const auto& entry : fs::directory_iterator(l_output_path))
			result++;
		return result;
	};

	auto stream_transform_directory = [&](std::function<void(std::ifstream&, std::ofstream&)> a_transform)
	{
		for (const auto& entry : fs::directory_iterator(l_input_path))
		{
			fs::path l_output_file_path(l_output_path.u8string() + std::to_string(num_output_files()));

			std::ifstream l_ifs(entry.path(), std::ios::in | std::ios::binary);
			std::ofstream l_ofs_e(l_output_file_path, std::ios::out | std::ios::trunc | std::ios::binary);

			a_transform(l_ifs, l_ofs_e);

			l_ifs.close();
			l_ofs_e.close();
		}
	};

	char response = 0;
	std::cin.get(response);

	if (response == 'E')
	{
		RSA::PublicKey l_public_key;

		if (!rsa_try_import(l_public_key, "public.dat"))
		{
			std::cout << "Error: public.dat not found." << std::endl;
			std::cin.get();
			return 0;
		}

		l_input_path = l_original_path;
		l_output_path = l_encrypt_path;
		stream_transform_directory(
			[&](std::ifstream& a_ifs, std::ofstream& a_ofs)
			{
				rsa_encrypt(a_ifs, a_ofs, l_public_key);
			}
		);
	}
	else if (response == 'D')
	{
		RSA::PrivateKey l_private_key;

		if (!rsa_try_import(l_private_key, "private.dat"))
		{
			std::cout << "Error: private.dat not found." << std::endl;
			std::cin.get();
			return 0;
		}

		l_input_path = l_encrypt_path;
		l_output_path = l_decrypt_path;
		stream_transform_directory(
			[&](std::ifstream& a_ifs, std::ofstream& a_ofs)
			{
				rsa_decrypt(a_ifs, a_ofs, l_private_key);
			}
		);
	}
	else
	{
		return 0;
	}

	return EXIT_SUCCESS;

}

class custom_class
{
public:
	int m_int;


public:
	virtual ~custom_class()
	{
		//std::cout << "DECONSTRUCTED" << std::endl;
	}
	custom_class(
		const int& a_int
	) :
		m_int(a_int)
	{
		//std::cout << "CONSTRUCTED" << std::endl;
	}

};

enum class test_enum : uint8_t
{
	unknown = 0,
	value1,
	value2
};

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;



 	return EXIT_SUCCESS;

}
