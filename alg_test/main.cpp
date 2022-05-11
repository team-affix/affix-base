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

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;
	using namespace affix_base::distributed_computing;

	remote_invocation_processor<std::string, double> l_remote_invocation_processor;

	l_remote_invocation_processor.add_function<int, std::string>("print_string",
		std::function([](double a_processing_assistance_arg, std::string a_string)
		{
			std::cout << a_processing_assistance_arg << std::endl;
			std::cout << a_string << std::endl;
			return EXIT_SUCCESS;
		}));

	remote_function_invoker<std::string, uint64_t> l_remote_function_invoker;
	
	byte_buffer l_result = l_remote_invocation_processor.process("print_string", 0.3, l_remote_function_invoker.argue<std::string>("test"));

	std::future<int> l_async_future = l_remote_function_invoker.async_get_result<int>(
		[&]
		{
			return l_result;
		});

	int l_async_result = l_async_future.get();

 	return EXIT_SUCCESS;

}
