#include "affix_base.h"
#include <filesystem>

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

	rsa_key_pair l_rsa_key_pair_0 = rsa_generate_key_pair(2048);
	rsa_key_pair l_rsa_key_pair_1 = rsa_generate_key_pair(2048);



 	return EXIT_SUCCESS;

}
