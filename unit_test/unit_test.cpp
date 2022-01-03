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
		void authenticate_local_test(
			asio::io_context& a_context,
			affix_base::networking::socket_io_guard& a_socket_io_guard,
			affix_base::cryptography::rsa_key_pair a_local_key_pair,
			const size_t& a_expected_seed_size
		)
		{
			std::function<void(bool)> l_callback = [&](bool a_result)
			{
				Assert::IsTrue(a_result);
			};

			affix_base::networking::async_authenticate_local l_auth(
				a_socket_io_guard
			);

			l_auth.start(
				a_local_key_pair,
				a_expected_seed_size,
				l_callback);

			a_context.run();

		}
		void authenticate_remote_test(
			asio::io_context& a_context,
			affix_base::networking::socket_io_guard& a_socket_io_guard,
			const std::vector<uint8_t>& a_remote_seed
		)
		{
			CryptoPP::AutoSeededRandomPool l_random;

			affix_base::data::ptr<affix_base::networking::async_authenticate_remote> l_auth = nullptr;

			std::function<void(bool)> l_callback = [&](bool a_result)
			{
				Assert::IsTrue(a_result);
				Assert::IsTrue(l_auth->m_remote_public_key.Validate(l_random, 3));
			};

			l_auth = new affix_base::networking::async_authenticate_remote(
				a_socket_io_guard
			);

			l_auth->start(
				a_remote_seed,
				l_callback);

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
			affix_base::data::ptr<asio::ip::tcp::socket> l_socket_0;
			asio::ip::tcp::socket l_socket_1(l_context_1);

			l_acceptor.async_accept(
				[&](asio::error_code a_ec, asio::ip::tcp::socket a_socket)
				{
					std::string a_ec_str = a_ec.message();
					l_socket_0 = new asio::ip::tcp::socket(std::move(a_socket));
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

			affix_base::networking::socket_io_guard l_socket_io_guard_0(*l_socket_0);
			affix_base::networking::socket_io_guard l_socket_io_guard_1(l_socket_1);

			// TEST COMMS BETWEEN SOCKETS

			std::vector<uint8_t> l_send_data(10);
			l_socket_io_guard_0.async_send(l_send_data, [&](bool a_result)
				{
					Assert::IsTrue(a_result);
				});

			std::vector<uint8_t> l_recv_data;
			l_socket_io_guard_1.async_receive(l_recv_data, [&](bool a_result)
				{
					Assert::IsTrue(a_result);
				});

			std::thread l_test_comms_thread_0(
				[&]
				{
					l_context_0.restart();
					l_context_0.run();
				});
			
			std::thread l_test_comms_thread_1(
				[&]
				{
					l_context_1.restart();
					l_context_1.run();
				});
			if (l_test_comms_thread_0.joinable())
				l_test_comms_thread_0.join();

			if (l_test_comms_thread_1.joinable())
				l_test_comms_thread_1.join();

			affix_base::cryptography::rsa_key_pair l_local_key_pair = affix_base::cryptography::rsa_generate_key_pair(a_key_size);
			
			size_t l_expected_seed_size = 25;

			std::vector<CryptoPP::byte> l_remote_seed(l_expected_seed_size);

			CryptoPP::AutoSeededRandomPool l_random;
			l_random.GenerateBlock(l_remote_seed.data(), l_remote_seed.size());
			
			std::thread l_thread_0(
				[&]
				{
					l_context_0.restart();
					authenticate_remote_test(
						l_context_0,
						l_socket_io_guard_0,
						l_remote_seed
					);
				});

			std::thread l_thread_1(
				[&]
				{
					l_context_1.restart();
					authenticate_local_test(
						l_context_1,
						l_socket_io_guard_1,
						l_local_key_pair,
						l_expected_seed_size
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
