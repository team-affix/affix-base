#pragma once
#include "pch.h"
#include "async_authenticate_remote.h"
#include "async_authenticate_local.h"

namespace affix_base
{
	namespace networking
	{
		class async_authenticate
		{
		protected:
			async_authenticate_remote m_authenticate_remote;
			async_authenticate_local m_authenticate_local;
			bool m_authenticate_remote_first = false;

		public:
			std::function<void(bool)> m_callback;

		public:
			async_authenticate(
				socket_io_guard& a_socket_io_guard
			);

		public:
			void start(
				const std::vector<uint8_t>& a_remote_seed,
				const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
				const bool& a_authenticate_remote_first,
				const std::function<void(bool)>& a_callback
			);

		};
	}
}
