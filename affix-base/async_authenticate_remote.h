#pragma once
#include "pch.h"
#include "transmission.h"
#include "socket_io_guard.h"
#include "rsa.h"

namespace affix_base {
	namespace networking {

		class async_authenticate_remote
		{
		public:
			socket_io_guard& m_socket_io_guard;

		public:
			std::vector<uint8_t> m_remote_seed;
			CryptoPP::RSA::PublicKey m_remote_public_key;

		public:
			std::function<void(bool)> m_callback;

		public:
			virtual ~async_authenticate_remote(

			);
			async_authenticate_remote(
				socket_io_guard& a_socket_io_guard,
				const std::vector<uint8_t>& a_remote_seed,
				const std::function<void(bool)>& a_callback
			);

		protected:
			void async_send_seed();
			void async_recv_auth();

		};

	}
}
