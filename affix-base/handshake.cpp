#include "pch.h"
#include "handshake.h"

using namespace affix_base;
using networking::async_handshake;

async_handshake::async_handshake(
	socket_io_guard& a_socket_io_guard,
	const cryptography::rsa_key_pair& a_local_key_pair,
	const std::vector<uint8_t>& a_remote_seed,
	const std::function<void(bool)>& a_callback) :
	m_socket_io_guard(a_socket_io_guard),
	m_local_key_pair(a_local_key_pair),
	m_remote_seed(a_remote_seed)
{
	async_send_seed();
	async_recv_seed();
}

void async_handshake::async_send_seed()
{
	byte_buffer l_byte_buffer;
	l_byte_buffer.push_back(m_remote_seed);

	// SEND DATA
	m_socket_io_guard.async_send(l_byte_buffer.data(),
		[&](bool a_result)
		{
			if (!a_result)
			{
				failure_callback();
				return;
			}

			send_seed_callback();

		});

}

void async_handshake::async_send_auth() {

	byte_buffer l_byte_buffer;

	// PUSH SEED ONTO BUFFER
	l_byte_buffer.push_back(m_local_seed);

	std::vector<uint8_t> l_signature;

	if (!cryptography::rsa_try_sign(m_local_seed, m_local_key_pair.private_key, l_signature))
	{
		failure_callback();
		return;
	}

	// PUSH SIGNATURE ONTO BUFFER
	l_byte_buffer.push_back(l_signature);

	// EXPORT LOCAL PUBLIC KEY TO BYTES
	std::vector<uint8_t> l_exported_rsa_param;
	cryptography::rsa_export(m_local_key_pair.public_key, l_exported_rsa_param);

	// PUSH LOCAL PUBLIC KEY ONTO BUFFER
	l_byte_buffer.push_back(l_exported_rsa_param);

	m_socket_io_guard.async_send(l_byte_buffer.data(),
		[&](bool a_result)
		{
			if (!a_result)
			{
				failure_callback();
				return;
			}

			send_auth_callback();

		});

}

void async_handshake::async_recv_seed() {

	ptr<vector<uint8_t>> l_data = new vector<uint8_t>();

	m_socket_io_guard.async_receive(*l_data,
		[&,l_data](bool a_result)
		{
			if (!a_result)
			{
				failure_callback();
				return;
			}

			byte_buffer l_byte_buffer(*l_data);
			l_byte_buffer.pop_front(m_local_seed);
			
			// SEEDS MUST MATCH IN SIZE
			if (m_local_seed.size() != m_remote_seed.size())
			{
				failure_callback();
				return;
			}

			recv_seed_callback();

		});
}

void async_handshake::async_recv_auth() {

	ptr<vector<uint8_t>> l_data = new vector<uint8_t>();

	m_socket_io_guard.async_receive(*l_data, 
		[&](bool a_result)
		{
			if (!a_result)
			{
				failure_callback();
				return;
			}

			byte_buffer l_byte_buffer(*l_data);

			vector<uint8_t> l_remote_public_key;
			vector<uint8_t> l_signature;
			vector<uint8_t> l_remote_seed;

			// POP PUBLIC KEY OFF OF BUFFER
			if (!l_byte_buffer.pop_back(l_remote_public_key))
			{
				failure_callback();
				return;
			}

			// TRY TO IMPORT REMOTE PUBLIC KEY
			if (!cryptography::rsa_try_import(m_remote_public_key, l_remote_public_key))
			{
				failure_callback();
				return;
			}

			// POP SIGNATURE OFF OF BUFFER
			if(!l_byte_buffer.pop_back(l_signature))
			{
				failure_callback();
				return;
			}

			// POP REMOTE SEED OFF OF BUFFER
			if(!l_byte_buffer.pop_back(l_remote_seed))
			{
				failure_callback();
				return;
			}

			// ENSURE THE KNOWN REMOTE SEED EQUALS MEASURED REMOTE SEED
			if (!std::equal(l_remote_seed.begin(), l_remote_seed.end(), m_remote_seed.begin(), m_remote_seed.end()))
			{
				failure_callback();
				return;
			}

			bool l_valid_signature = false;

			if (cryptography::rsa_try_verify(l_remote_seed, l_signature, m_remote_public_key, l_valid_signature))
			{
				failure_callback();
				return;
			}

			// ENSURE VALID SIGNATURE
			if (!l_valid_signature)
			{
				failure_callback();
				return;
			}

			recv_auth_callback();

		});

}

void async_handshake::failure_callback() {
	
	m_progress_mutex.lock();
	bool l_failure_callback_already_executed = m_failed;
	m_failed = true;
	m_progress_mutex.unlock();

	if (!l_failure_callback_already_executed) {
		m_callback(false);
	}

}

void async_handshake::send_seed_callback() {

	m_progress_mutex.lock();
	if (!m_failed)
	{
		async_recv_auth();
	}
	m_progress_mutex.unlock();

}

void async_handshake::send_auth_callback() {

	m_progress_mutex.lock();
	if (!m_failed)
	{
		m_local_authenticated = true;
	}
	m_progress_mutex.unlock();

	if (m_local_authenticated && m_remote_authenticated)
	{
		m_callback(true);
	}

}

void async_handshake::recv_seed_callback() {

	m_progress_mutex.lock();
	if (!m_failed)
	{
		async_send_auth();
	}
	m_progress_mutex.unlock();

}

void async_handshake::recv_auth_callback() {

	m_progress_mutex.lock();
	if (!m_failed)
	{
		m_remote_authenticated = true;
	}
	m_progress_mutex.unlock();

	if (m_remote_authenticated && m_local_authenticated)
	{
		m_callback(true);
	}

}
