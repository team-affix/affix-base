#include "pch.h"
#include "async_authenticate_local.h"

using namespace affix_base;
using networking::async_authenticate_local;
using affix_base::data::byte_buffer;
using affix_base::data::ptr;
using std::vector;
using std::lock_guard;
using affix_base::threading::cross_thread_mutex;

async_authenticate_local::~async_authenticate_local(

)
{
	lock_guard<cross_thread_mutex> l_receive_lock_guard(m_socket_io_guard.m_receive_mutex);
	lock_guard<cross_thread_mutex> l_send_lock_guard(m_socket_io_guard.m_send_mutex);
}

async_authenticate_local::async_authenticate_local(
	socket_io_guard& a_socket_io_guard,
	const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
	const size_t& a_expected_seed_size,
	const std::function<void(bool)>& a_callback
) :
	m_socket_io_guard(a_socket_io_guard),
	m_local_key_pair(a_local_key_pair),
	m_expected_seed_size(a_expected_seed_size),
	m_callback(a_callback)
{
	async_recv_seed();
}

void async_authenticate_local::async_recv_seed()
{
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
			l_byte_buffer.pop_front(m_local_seed);

			// SEEDS MUST MATCH IN SIZE
			if (m_local_seed.size() != m_expected_seed_size)
			{
				m_callback(false);
				return;
			}

			async_send_auth();

		});

}

void async_authenticate_local::async_send_auth()
{
	byte_buffer l_byte_buffer;

	// PUSH SEED ONTO BUFFER
	l_byte_buffer.push_back(m_local_seed);

	std::vector<uint8_t> l_signature;

	if (!cryptography::rsa_try_sign(m_local_seed, m_local_key_pair.private_key, l_signature))
	{
		m_callback(false);
		return;
	}

	// PUSH SIGNATURE ONTO BUFFER
	l_byte_buffer.push_back(l_signature);

	// EXPORT LOCAL PUBLIC KEY TO BYTES
	std::vector<uint8_t> l_exported_rsa_param;
	cryptography::rsa_export(m_local_key_pair.public_key, l_exported_rsa_param);

	// PUSH LOCAL PUBLIC KEY ONTO BUFFER
	l_byte_buffer.push_back(l_exported_rsa_param);

	m_socket_io_guard.async_send(l_byte_buffer.data(), m_callback);

}
