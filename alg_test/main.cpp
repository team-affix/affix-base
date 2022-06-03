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

struct function_data
{
	std::string m_invocation_identifier;
	affix_base::data::byte_buffer m_serialized_data;
};

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;
	using namespace affix_base::distributed_computing;

	uint64_t l_start_time = affix_base::timing::utc_time();

	auto l_continue = [&] { return affix_base::timing::utc_time() - l_start_time < 10; };

	std::thread l_thread([&]
		{
			while (l_continue())
			{
				affix_base::data::ptr<double> l_ptr(new double(10.505));
			}
		});

	while (l_continue())
	{
		affix_base::data::ptr<double> l_ptr(new double(11.506));
	}

	if (l_thread.joinable())
		l_thread.join();
	
 	return EXIT_SUCCESS;

}
