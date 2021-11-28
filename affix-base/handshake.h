#pragma once
#include "pch.h"
#include "transmission.h"
#include "socket_io_guard.h"
#include "rsa.h"

namespace affix_base {
	namespace networking {

		class async_handshake {
		protected:
			std::mutex m_progress_mutex;
			bool m_failed = false;
			bool m_local_authenticated = false;
			bool m_remote_authenticated = false;

		protected:
			socket_io_guard& m_socket_io_guard;

		protected:
			cryptography::rsa_key_pair m_local_key_pair;
			std::vector<uint8_t> m_remote_seed;

		public:
			CryptoPP::RSA::PublicKey m_remote_public_key;
			std::vector<uint8_t> m_local_seed;

		public:
			std::function<void(bool)> m_callback;

		public:
			async_handshake(
				socket_io_guard& a_socket_io_guard,
				const cryptography::rsa_key_pair& a_local_key_pair,
				const std::vector<uint8_t>& a_remote_seed,
				const std::function<void(bool)>& a_callback);

		protected:
			void async_send_seed();
			void async_send_auth();
			void async_recv_seed();
			void async_recv_auth();

		protected:
			void failure_callback();
			void send_seed_callback();
			void send_auth_callback();
			void recv_seed_callback();
			void recv_auth_callback();

		};

	}
}
