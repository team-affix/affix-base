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

	remote_function_invoker<std::string> l_remote_function_invoker;

	remote_invocation_processor<std::string, int> l_remote_invocation_processor;

	l_remote_invocation_processor.add_function("remote_function",
		std::function([](int a_processing_assistance_arg, std::string a_text)
			{
				std::cout << a_processing_assistance_arg << ", " << a_text << std::endl;
			}));

	affix_base::data::byte_buffer l_byte_buffer;

	if (!l_remote_function_invoker.serialize_invocation(l_byte_buffer, "remote_function"))
	{
		std::cout << "Unable to serialize invocation" << std::endl;
	}

	if (!l_remote_invocation_processor.process(10, l_byte_buffer))
	{
		std::cout << "Unable to deserialize invocation" << std::endl;
	}


 	return EXIT_SUCCESS;

}
