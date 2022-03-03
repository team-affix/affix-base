#include "affix_base.h"
#include <filesystem>
#include "serializable.h"

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

struct simple_struct : affix_base::data::serializable
{
	std::string m_string = "hello world";
	int m_int = 3;

	simple_struct(

	) :
		affix_base::data::serializable(m_string, m_int)
	{

	}

	simple_struct(
		const simple_struct& a_other
	) :
		affix_base::data::serializable(m_string, m_int)
	{

	}

};

struct simpler_struct
{
	int i1 = 1;
	int i2 = 2;
};

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;

	byte_buffer l_byte_buffer;

	std::map<std::string, std::string> l_map = {
		{"jake", "thebest2"},
		{"gianna", "thebest"}
	};

	l_byte_buffer.push_front(l_map);

	std::map<std::string, std::string> l_map_2;

	l_byte_buffer.pop_back(l_map_2);

 	return EXIT_SUCCESS;

}
