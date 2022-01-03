#include "pch.h"
#include "async_authenticate.h"

using affix_base::networking::async_authenticate;

async_authenticate::async_authenticate(
	socket_io_guard& a_socket_io_guard
) :
	m_authenticate_remote(a_socket_io_guard),
	m_authenticate_local(a_socket_io_guard)
{

}

void async_authenticate::start(
	const std::vector<uint8_t>& a_remote_seed,
	const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
	const bool& a_authenticate_remote_first,
	const std::function<void(bool)>& a_callback
)
{
	if (m_authenticate_remote_first)
	{
		m_authenticate_remote.start(
			a_remote_seed,
			[&, a_local_key_pair, a_remote_seed, a_callback](bool a_result)
			{
				if (a_result)
				{
					m_authenticate_local.start(
						a_local_key_pair,
						a_remote_seed.size(),
						a_callback
					);
				}
				else
				{
					a_callback(false);
				}
			});

	}
	else
	{
		m_authenticate_local.start(
			a_local_key_pair,
			a_remote_seed.size(),
			[&, a_remote_seed, a_callback](bool a_result)
			{
				if (a_result)
				{
					m_authenticate_remote.start(
						a_remote_seed,
						a_callback
					);
				}
				else
				{
					a_callback(false);
				}
			}
		);

	}
}

