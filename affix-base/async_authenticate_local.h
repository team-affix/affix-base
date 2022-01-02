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
		protected:
			socket_io_guard& m_socket_io_guard;

		protected:
			std::vector<uint8_t> m_local_seed;
			affix_base::cryptography::rsa_key_pair m_local_key_pair;

		protected:
			std::function<void(bool)> m_callback;

		public:
			async_authenticate_local(
				socket_io_guard& a_socket_io_guard,
				affix_base::cryptography::rsa_key_pair a_local_key_pair,
				const std::function<void(bool)>& a_callback
			);

		protected:
			void async_recv_seed();
			void async_send_auth();

		};
	}
}
