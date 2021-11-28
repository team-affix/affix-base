#include "pch.h"
#include "CppUnitTest.h"
#include "affix-base/rsa.h"
#include "affix-base/networking.h"
#include "asio/asio/include/asio.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(test_async_handshake)
	{
	protected:
		asio::ip::tcp::socket start_socket(asio::io_context& a_context)
		{
			asio::ip::tcp::socket l_result(a_context);
			asio::ip::tcp::endpoint l_local_endpoint(asio::ip::tcp::v4(), 0);
			l_result.bind(l_local_endpoint);
			return l_result;
		}

	protected:
		void handshake_test(uint16_t a_key_size)
		{
			asio::io_context l_context;

			asio::ip::tcp::socket l_socket_0 = start_socket(l_context);
			asio::ip::tcp::socket l_socket_1 = start_socket(l_context);

			affix_base::networking::socket_io_guard l_socket_io_guard_0(l_socket_0);
			affix_base::networking::socket_io_guard l_socket_io_guard_1(l_socket_1);
			
			affix_base::cryptography::rsa_key_pair l_key_pair_0 = affix_base::cryptography::rsa_generate_key_pair(a_key_size);
			affix_base::cryptography::rsa_key_pair l_key_pair_1 = affix_base::cryptography::rsa_generate_key_pair(a_key_size);


		}

	public:
		TEST_METHOD(rsa_2048_test)
		{
			handshake_test(2048);
		}
		TEST_METHOD(rsa_4096_test)
		{
			handshake_test(4096);
		}

	};
}
