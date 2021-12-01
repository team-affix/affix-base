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
		const wchar_t* to_const_wchar_t(const std::string& str)
		{
			std::wostringstream wstm;
			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
			for (size_t i = 0; i < str.size(); ++i)
				wstm << ctfacet.widen(str[i]);
			return wstm.str().c_str();
		}
	protected:
		asio::ip::tcp::socket start_socket(asio::io_context& a_context)
		{
			asio::ip::tcp::socket l_result(a_context);
			return l_result;
		}
		asio::ip::tcp::acceptor start_aceptor(asio::io_context& a_context)
		{
			asio::ip::tcp::endpoint l_local_endpoint(asio::ip::tcp::v4(), 8091);
			asio::ip::tcp::acceptor l_result(a_context, l_local_endpoint);
			return l_result;
		}

	protected:
		void handshake_test(uint16_t a_key_size)
		{
			asio::io_context l_context;

			asio::ip::tcp::socket l_socket_0 = start_socket(l_context);
			asio::ip::tcp::acceptor l_acceptor = start_aceptor(l_context);

			bool l_connected = false;
			bool l_accepted = false;

			affix_base::data::ptr<asio::ip::tcp::socket> l_socket_1 = nullptr;
			l_acceptor.async_accept(
				[&](asio::error_code a_ec, asio::ip::tcp::socket a_socket)
				{
					Assert::IsFalse((bool)a_ec, to_const_wchar_t("Error accepting connection using asio::ip::tcp::acceptor."));
					l_accepted = true;
					l_socket_1 = new asio::ip::tcp::socket(std::move(a_socket));
					return;
				});
			l_socket_0.async_connect(l_acceptor.local_endpoint(),
				[&](asio::error_code a_ec)
				{
					Assert::IsFalse((bool)a_ec, to_const_wchar_t("Error connecting using asio::ip::tcp::socket."));
					l_connected = true;
					return;
				});

			l_context.run();

			affix_base::networking::socket_io_guard l_socket_io_guard_0(l_socket_0);
			affix_base::networking::socket_io_guard l_socket_io_guard_1(*l_socket_1);
			
			affix_base::cryptography::rsa_key_pair l_key_pair_0 = affix_base::cryptography::rsa_generate_key_pair(a_key_size);
			affix_base::cryptography::rsa_key_pair l_key_pair_1 = affix_base::cryptography::rsa_generate_key_pair(a_key_size);

			std::vector<CryptoPP::byte> l_remote_seed_0;
			std::vector<CryptoPP::byte> l_remote_seed_1;

			// GENERATE RANDOM SEEDS
			CryptoPP::AutoSeededRandomPool l_random;
			l_random.GenerateBlock(l_remote_seed_0.data(), 25);
			l_random.GenerateBlock(l_remote_seed_1.data(), 25);

			affix_base::data::ptr<affix_base::networking::async_handshake> ash_0 = nullptr;
			affix_base::data::ptr<affix_base::networking::async_handshake> ash_1 = nullptr;

			bool l_callback_0_fired = false;
			bool l_callback_1_fired = false;

			// CREATE CALLBACK FN'S
			auto l_callback_0 = [&](bool a_result)
			{
				// VERIFY THAT REMOTE SEED TRANSFERS SUCCESSFULLY
				Assert::IsTrue(std::equal(l_remote_seed_1.begin(), l_remote_seed_1.end(), ash_0->m_local_seed.begin(), ash_0->m_local_seed.end()), (const wchar_t*)"Received seed mismatch.");
				// VERIFY THAT THE PUBLIC KEY WAS IN A VALID FORM AND TRANSMITTED PROPERLY
				Assert::IsTrue(ash_0->m_remote_public_key.Validate(l_random, 3), (const wchar_t*)"Remote public key ill-formed.");
				// VERIFY THAT NO MISC. ERRORS OCCURED
				Assert::IsTrue(a_result, (const wchar_t*)"Misc error.");
				l_callback_0_fired = true;
			};
			auto l_callback_1 = [&](bool a_result)
			{
				// VERIFY THAT REMOTE SEED TRANSFERS SUCCESSFULLY
				Assert::IsTrue(std::equal(l_remote_seed_0.begin(), l_remote_seed_0.end(), ash_1->m_local_seed.begin(), ash_1->m_local_seed.end()), (const wchar_t*)"Received seed mismatch.");
				// VERIFY THAT THE PUBLIC KEY WAS IN A VALID FORM AND TRANSMITTED PROPERLY
				Assert::IsTrue(ash_1->m_remote_public_key.Validate(l_random, 3), (const wchar_t*)"Remote public key ill-formed.");
				// VERIFY THAT NO MISC. ERRORS OCCURED
				Assert::IsTrue(a_result, (const wchar_t*)"Misc error.");
				l_callback_1_fired = true;
			};

			ash_0 = new affix_base::networking::async_handshake(l_socket_io_guard_0, l_key_pair_0, l_remote_seed_0, l_callback_0);
			ash_1 = new affix_base::networking::async_handshake(l_socket_io_guard_1, l_key_pair_1, l_remote_seed_1, l_callback_1);

			// WAIT FOR CALLBACKS TO FIRE
			l_context.run();

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
