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
		asio::ip::tcp::endpoint m_acceptor_endpoint = asio::ip::tcp::endpoint(asio::ip::make_address("192.168.1.141"), 8090);

	protected:
		void handshake_test(
			asio::io_context& a_context,
			asio::ip::tcp::socket& a_socket,
			const affix_base::cryptography::rsa_key_pair& a_key_pair,
			const std::vector<CryptoPP::byte>& a_remote_seed,
			const std::vector<CryptoPP::byte>& a_local_seed,
			const bool& a_send_first
		)
		{
			affix_base::networking::socket_io_guard l_socket_io_guard(a_socket);

			affix_base::data::ptr<affix_base::networking::async_authenticate_remote> ash = nullptr;
			
			CryptoPP::AutoSeededRandomPool l_random;

			auto l_callback = [&](bool a_result)
			{
				// VERIFY THAT REMOTE SEED TRANSFERS SUCCESSFULLY
				Assert::IsTrue(std::equal(a_local_seed.begin(), a_local_seed.end(), ash->m_local_seed.begin(), ash->m_local_seed.end()));
				// VERIFY THAT THE PUBLIC KEY WAS IN A VALID FORM AND TRANSMITTED PROPERLY
				Assert::IsTrue(ash->m_remote_public_key.Validate(l_random, 3));
				// VERIFY THAT NO MISC. ERRORS OCCURED
				Assert::IsTrue(a_result);
			};

			ash = new affix_base::networking::async_authenticate_remote(
				l_socket_io_guard,
				a_key_pair,
				a_remote_seed,
				l_callback,
				a_send_first
			);

			a_context.run();

		}

	protected:
		void handshake_test(
			const uint16_t& a_key_size
		)
		{
			asio::io_context l_context_0;
			asio::io_context l_context_1;

			asio::ip::tcp::acceptor l_acceptor(l_context_0, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), m_acceptor_endpoint.port()));
			asio::ip::tcp::socket l_socket_0(l_context_0);
			asio::ip::tcp::socket l_socket_1(l_context_1);

			l_acceptor.async_accept(
				[&](asio::error_code a_ec, asio::ip::tcp::socket a_socket)
				{
					l_socket_0 = std::move(a_socket);
				});

			std::thread l_connect_thread_0(
				[&]
				{
					l_context_0.run();
				});
			std::thread l_connect_thread_1(
				[&]
				{
					l_socket_1.connect(m_acceptor_endpoint);
				});

			if (l_connect_thread_0.joinable())
				l_connect_thread_0.join();

			if(l_connect_thread_1.joinable())
				l_connect_thread_1.join();

			affix_base::cryptography::rsa_key_pair l_key_pair_0 = affix_base::cryptography::rsa_generate_key_pair(a_key_size);
			affix_base::cryptography::rsa_key_pair l_key_pair_1 = affix_base::cryptography::rsa_generate_key_pair(a_key_size);

			std::vector<CryptoPP::byte> l_remote_seed_0(25);
			std::vector<CryptoPP::byte> l_remote_seed_1(25);

			CryptoPP::AutoSeededRandomPool l_random;
			l_random.GenerateBlock(l_remote_seed_0.data(), l_remote_seed_0.size());
			l_random.GenerateBlock(l_remote_seed_1.data(), l_remote_seed_1.size());

			std::thread l_thread_0(
				[&]
				{
					handshake_test(
						l_context_0,
						l_socket_0,
						l_key_pair_0,
						l_remote_seed_0,
						l_remote_seed_1,
						true
					);
				});

			std::thread l_thread_1(
				[&]
				{
					handshake_test(
						l_context_1,
						l_socket_1,
						l_key_pair_1,
						l_remote_seed_1,
						l_remote_seed_0,
						false
					);
				});

			if (l_thread_0.joinable())
				l_thread_0.join();

			if (l_thread_1.joinable())
				l_thread_1.join();

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
