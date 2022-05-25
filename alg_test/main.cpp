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

	bool l_exit_code_0 = affix_base::files::file_write("test_file.txt", std::string("hello, this is a test"));


	std::string l_result;

	bool l_exit_code_1 = affix_base::files::file_read("test_file.txt", l_result);

 	return EXIT_SUCCESS;

}
