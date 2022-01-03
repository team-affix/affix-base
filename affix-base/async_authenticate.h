#pragma once
#include "pch.h"
#include "async_authenticate_remote.h"
#include "async_authenticate_local.h"
#include "ptr.h"

namespace affix_base
{
	namespace networking
	{
		class async_authenticate
		{
		protected:
			size_t m_authentication_count = -1;

		public:
			affix_base::data::ptr<async_authenticate_remote> m_authenticate_remote;
			affix_base::data::ptr<async_authenticate_local> m_authenticate_local;

		protected:
			socket_io_guard& m_socket_io_guard;
			std::vector<uint8_t> m_remote_seed;
			affix_base::cryptography::rsa_key_pair m_local_key_pair;
			bool m_authenticate_remote_first = false;
			std::function<void(bool)> m_callback;

		public:
			async_authenticate(
				socket_io_guard& a_socket_io_guard,
				const std::vector<uint8_t>& a_remote_seed,
				const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
				const bool& a_authenticate_remote_first,
				const std::function<void(bool)>& a_callback
			);

		protected:
			void authenticate_remote();
			void authenticate_local();

		protected:
			void authenticate_next();

		};
	}
}
