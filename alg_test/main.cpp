#include "affix_base.h"
#include <filesystem>
#include "serializable.h"
#include <tuple>
#include "cryptopp/osrng.h"
#include "guarded_resource.h"

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

struct my_serializable_struct : public affix_base::data::serializable
{
	std::string m_string = "";
	int m_int = 0;
	std::vector<double> m_vector = { };

	my_serializable_struct(

	) :
		affix_base::data::serializable(m_string, m_int, m_vector)
	{

	}

	my_serializable_struct(
		const std::string& a_string,
		const int& a_int,
		const std::vector<double>& a_vector
	) :
		affix_base::data::serializable(m_string, m_int, m_vector),
		m_string(a_string),
		m_int(a_int),
		m_vector(a_vector)
	{

	}

};

int main(

)
{
	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;
	using namespace affix_base::distributed_computing;

	affix_base::data::byte_buffer l_byte_buffer;

	my_serializable_struct l_struct("testing123", 13, { 1.2, 2.3, 3.4, 4.5 });

	l_byte_buffer.push_back(l_struct);

	my_serializable_struct l_struct_2;

	l_byte_buffer.pop_front(l_struct_2);

 	return EXIT_SUCCESS;

}
