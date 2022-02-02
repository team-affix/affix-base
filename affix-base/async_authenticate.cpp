#include "pch.h"
#include "async_authenticate.h"

using affix_base::networking::async_authenticate;
using std::lock_guard;
using affix_base::threading::cross_thread_mutex;

async_authenticate::~async_authenticate(

)
{

}

async_authenticate::async_authenticate(
	socket_io_guard& a_socket_io_guard,
	const std::vector<uint8_t>& a_remote_seed,
	const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
	const bool& a_authenticate_remote_first,
	const std::function<void(bool)>& a_callback
) :
	m_socket_io_guard(a_socket_io_guard),
	m_remote_seed(a_remote_seed),
	m_local_key_pair(a_local_key_pair),
	m_authenticate_remote_first(a_authenticate_remote_first),
	m_callback(a_callback)
{
	authenticate_next();
}

void async_authenticate::authenticate_remote()
{
	m_authenticate_remote = new async_authenticate_remote(
		m_socket_io_guard,
		m_remote_seed,
		[&](bool a_result)
		{
			if (a_result)
			{
				authenticate_next();
			}
			else
			{
				m_callback(false);
			}

		});

}

void async_authenticate::authenticate_local()
{
	m_authenticate_local = new async_authenticate_local(
		m_socket_io_guard,
		m_local_key_pair,
		m_remote_seed.size(),
		[&](bool a_result)
		{
			if (a_result)
			{
				authenticate_next();
			}
			else
			{
				m_callback(false);
			}

		});

}

void async_authenticate::authenticate_next()
{
	// m_authentication_count STARTS AT -1, THEREFORE INCREMENTING INITIALLY RAISES TO ZERO
	m_authentication_count++;

	switch (m_authentication_count)
	{
	case 0:

		// NO AUTHENTICATIONS HAVE PROCESSED
		if (m_authenticate_remote_first)
			authenticate_remote();
		else
			authenticate_local();

		break;
	case 1:

		// ONLY A SINGLE AUTHENTICATION (LOCAL or REMOTE) HAS PROCESSED.
		// NOW, AUTHENTICATE THE REMAINING SIDE
		if (m_authenticate_remote_first)
			authenticate_local();
		else
			authenticate_remote();

		break;
	case 2:

		// BOTH AUTHENTICATIONS HAVE SUCCEEDED
		m_callback(true);

		break;
	default:

		// SOMETHING HAS FAILED MISERABLY, COUNT SHOULD MAX TO 2.
		throw std::exception("Error in async_authenticate: m_authentication_count went above 2.");

		break;
	}

}
