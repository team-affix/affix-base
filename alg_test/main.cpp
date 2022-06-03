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

	guarded_resource<std::vector<int>> l_guarded_resource_0;
	guarded_resource<std::vector<int>> l_guarded_resource_1;
	
	std::scoped_lock l_lock(l_guarded_resource_0, l_guarded_resource_1);

	l_guarded_resource_0->push_back(10);

 	return EXIT_SUCCESS;

}
