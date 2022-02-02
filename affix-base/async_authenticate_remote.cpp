#include "pch.h"
#include "async_authenticate_remote.h"
#include "byte_buffer.h"

using namespace affix_base;
using networking::async_authenticate_remote;
using affix_base::data::byte_buffer;
using affix_base::data::ptr;
using std::vector;
using std::lock_guard;
using affix_base::threading::cross_thread_mutex;

async_authenticate_remote::~async_authenticate_remote(

)
{
	lock_guard<cross_thread_mutex> l_receive_lock_guard(m_socket_io_guard.m_receive_mutex);
	lock_guard<cross_thread_mutex> l_send_lock_guard(m_socket_io_guard.m_send_mutex);
}

async_authenticate_remote::async_authenticate_remote(
	socket_io_guard& a_socket_io_guard,
	const std::vector<uint8_t>& a_remote_seed,
	const std::function<void(bool)>& a_callback
) :
	m_socket_io_guard(a_socket_io_guard),
	m_remote_seed(a_remote_seed),
	m_callback(a_callback)
{
	async_send_seed();
}

void async_authenticate_remote::async_send_seed()
{
	byte_buffer l_byte_buffer;

	l_byte_buffer.push_back(m_remote_seed);

	// SEND DATA
	m_socket_io_guard.async_send(l_byte_buffer.data(),
		[&](bool a_result)
		{
			if (a_result)
			{
				async_recv_auth();
			}
			else
			{
				m_callback(false);
			}

		});

}

void async_authenticate_remote::async_recv_auth() {

	ptr<vector<uint8_t>> l_data = new vector<uint8_t>();

	m_socket_io_guard.async_receive(*l_data, 
		[&, l_data](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			byte_buffer l_byte_buffer(*l_data);

			vector<uint8_t> l_remote_public_key;
			vector<uint8_t> l_signature;
			vector<uint8_t> l_remote_seed;

			// POP PUBLIC KEY OFF OF BUFFER
			if (!l_byte_buffer.pop_back(l_remote_public_key))
			{
				m_callback(false);
				return;
			}

			// TRY TO IMPORT REMOTE PUBLIC KEY
			if (!cryptography::rsa_try_import(m_remote_public_key, l_remote_public_key))
			{
				m_callback(false);
				return;
			}

			// POP SIGNATURE OFF OF BUFFER
			if(!l_byte_buffer.pop_back(l_signature))
			{
				m_callback(false);
				return;
			}

			// POP REMOTE SEED OFF OF BUFFER
			if(!l_byte_buffer.pop_back(l_remote_seed))
			{
				m_callback(false);
				return;
			}

			// ENSURE THE KNOWN REMOTE SEED EQUALS MEASURED REMOTE SEED
			if (!std::equal(l_remote_seed.begin(), l_remote_seed.end(), m_remote_seed.begin(), m_remote_seed.end()))
			{
				m_callback(false);
				return;
			}

			bool l_valid_signature = false;

			if (!cryptography::rsa_try_verify(l_remote_seed, l_signature, m_remote_public_key, l_valid_signature))
			{
				m_callback(false);
				return;
			}

			// ENSURE VALID SIGNATURE
			if (!l_valid_signature)
			{
				m_callback(false);
				return;
			}

			m_callback(true);

		});

}
