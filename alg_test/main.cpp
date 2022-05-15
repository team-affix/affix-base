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

	function_data l_data_to_processor;
	function_data l_data_to_invoker;

	remote_invocation_processor<std::string, int> l_remote_invocation_processor;

	l_remote_invocation_processor.add_function("print_string",
		std::function([&](int a_assistance_type, std::string a_string)
		{
			std::cout << a_assistance_type << ": " << a_string << std::endl;
		}));

	remote_function_invoker<std::string> l_remote_function_invoker;

	byte_buffer l_serialized_function_call = l_remote_function_invoker.serialize_invocation<std::string>(
		"print_string",
		"hello");

	l_remote_invocation_processor.process(10, l_serialized_function_call);

 	return EXIT_SUCCESS;

}
