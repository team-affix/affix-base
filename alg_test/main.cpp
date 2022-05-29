#include "affix_base.h"
#include <filesystem>
#include "serializable.h"
#include <tuple>
#include "cryptopp/osrng.h"
#include "guarded_resource.h"
#include "synchronized_resource.h"

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

	std::vector<uint8_t> l_hash_0;

	bool l_successful_0 = affix_base::files::file_hash("test.jpg", l_hash_0);

	std::vector<uint8_t> l_hash_1;

	bool l_successful_1 = affix_base::files::file_hash("test.jpg", l_hash_1);

	std::ofstream l_ofs("test.jpg", std::ios::out | std::ios::binary);

	l_ofs.seekp(1000000);

	l_ofs.write("1", 1);

	l_ofs.close();

	std::vector<uint8_t> l_hash_2;

	bool l_successful_2 = affix_base::files::file_hash("test.jpg", l_hash_2);



 	return EXIT_SUCCESS;

}
