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

class training_set
{
public:
	std::vector<double> m_x;
	std::vector<double> m_y;
	training_set(

	)
	{

	}
	training_set(
		const std::vector<double>& a_x,
		const std::vector<double>& a_y
	) :
		m_x(a_x),
		m_y(a_y)
	{

	}
	bool serialize(
		affix_base::data::byte_buffer& a_byte_buffer
	) const
	{
		if (!a_byte_buffer.push_back(m_x))
			return false;
		if (!a_byte_buffer.push_back(m_y))
			return false;
		return true;
	}
	bool deserialize(
		affix_base::data::byte_buffer& a_byte_buffer
	)
	{
		if (!a_byte_buffer.pop_front(m_x))
			return false;
		if (!a_byte_buffer.pop_front(m_y))
			return false;
		return true;
	}
};

int main(

)
{
    CryptoPP::AutoSeededRandomPool l_random;
    std::cout << l_random.GenerateWord32() << std::endl;
  	return EXIT_SUCCESS;

}
