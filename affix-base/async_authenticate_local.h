#pragma once
#include "pch.h"
#include "socket_io_guard.h"
#include "rsa.h"

namespace affix_base
{
	namespace networking
	{
		class async_authenticate_local
		{
		public:
			socket_io_guard& m_socket_io_guard;

		public:
			std::vector<uint8_t> m_local_seed;
			affix_base::cryptography::rsa_key_pair m_local_key_pair;
			size_t m_expected_seed_size = 0;

		public:
			std::function<void(bool)> m_callback;

		public:
			async_authenticate_local(
				socket_io_guard& a_socket_io_guard
			);

		public:
			void start(
				const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
				const size_t& a_expected_seed_size,
				const std::function<void(bool)>& a_callback
			);

		protected:
			void async_recv_seed();
			void async_send_auth();

		};
	}
}
