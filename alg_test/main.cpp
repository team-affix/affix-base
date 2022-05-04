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

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;

	byte_buffer l_byte_buffer;
	l_byte_buffer.push_back(double(35));

	guarded_resource<std::vector<uint8_t>> l_bytes(l_byte_buffer.data());

	synchronized_resource<double, std::vector<uint8_t>> l_sync(
		l_bytes,
		[](const std::vector<uint8_t>& a_remote)
		{
			double l_result = 0;
			byte_buffer l_pull_byte_buffer(a_remote);
			l_pull_byte_buffer.pop_front(l_result);
			return l_result;
		},
		[](const double& a_local)
		{
			byte_buffer l_push_byte_buffer;
			l_push_byte_buffer.push_back(a_local);
			return l_push_byte_buffer.data();
		});

	{
		locked_resource l_locked_resource = l_sync.lock();
		(*l_locked_resource)++;
	}

 	return EXIT_SUCCESS;

}
