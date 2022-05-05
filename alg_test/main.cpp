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

	byte_buffer l_byte_buffer;
	l_byte_buffer.push_back(double(35));
	
	guarded_resource<std::vector<uint8_t>> l_bytes(l_byte_buffer.data());

	synchronized_resource<double, std::vector<uint8_t>> l_sync_0(
		l_bytes,
		[](const std::vector<uint8_t>& a_remote, double& a_local)
		{
			byte_buffer l_pull_byte_buffer(a_remote);
			l_pull_byte_buffer.pop_front(a_local);
		},
		[](const double& a_local, std::vector<uint8_t>& a_remote)
		{
			byte_buffer l_push_byte_buffer;
			l_push_byte_buffer.push_back(a_local);
			a_remote = l_push_byte_buffer.data();
		});

	synchronized_resource<double, double> l_sync_1(
		l_sync_0,
		[](const double& a_remote, double& a_local)
		{
			a_local = a_remote;
		},
		[](const double& a_local, double& a_remote)
		{
			a_remote = a_local;
		});

	{
		locked_resource l_locked_resource = l_sync_1.lock();
		*l_locked_resource = 36;
	}

 	return EXIT_SUCCESS;

}
