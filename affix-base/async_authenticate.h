#pragma once
#include "pch.h"
#include "ptr.h"
#include "socket_io_guard.h"
#include "rsa.h"

namespace affix_base
{
	namespace networking
	{
		class async_authenticate
		{
		protected:
			socket_io_guard& m_socket_io_guard;
			bool m_identify_local_first = false;
			size_t m_authentication_count = -1;
			std::function<void(bool)> m_callback;

		public:
			affix_base::cryptography::rsa_key_pair m_local_key_pair;
			std::vector<uint8_t> m_local_seed;
			CryptoPP::RSA::PublicKey m_remote_public_key;
			std::vector<uint8_t> m_remote_seed;

		public:
			virtual ~async_authenticate(

			);
			async_authenticate(
				socket_io_guard& a_socket_io_guard,
				const std::vector<uint8_t>& a_remote_seed,
				const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
				const bool& a_identify_local_first,
				const std::function<void(bool)>& a_callback
			);

		protected:
			void send_identity(
			
			);

			void recv_identity(
			
			);

			void send_seed(
			
			);

			void recv_seed(
			
			);

			void send_auth(
			
			);

			void recv_auth(
			
			);

		};
	}
}
