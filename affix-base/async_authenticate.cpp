#include "pch.h"
#include "async_authenticate.h"

using affix_base::networking::async_authenticate;
using std::lock_guard;
using affix_base::threading::cross_thread_mutex;
using affix_base::data::byte_buffer;
using namespace affix_base::data;

async_authenticate::~async_authenticate(

)
{

}

async_authenticate::async_authenticate(
	socket_io_guard& a_socket_io_guard,
	const std::vector<uint8_t>& a_remote_seed,
	const affix_base::cryptography::rsa_key_pair& a_local_key_pair,
	const bool& a_identify_local_first,
	const std::function<void(bool)>& a_callback
) :
	m_socket_io_guard(a_socket_io_guard),
	m_remote_seed(a_remote_seed),
	m_local_key_pair(a_local_key_pair),
	m_identify_local_first(a_identify_local_first),
	m_callback(a_callback)
{
	if (a_identify_local_first)
	{
		send_identity();
	}
	else
	{
		recv_identity();
	}
}

void async_authenticate::send_identity(

)
{
	// Construct an empty byte buffer which will be populated with necessary information to send
	// to the remote peer.
	byte_buffer l_byte_buffer;

	// Export the local identity
	std::vector<uint8_t> l_exported_local_identity;
	affix_base::cryptography::rsa_export(m_local_key_pair.public_key, l_exported_local_identity);

	// Push the exported identity into the byte buffer
	l_byte_buffer.push_back(l_exported_local_identity);

	m_socket_io_guard.async_send(l_byte_buffer.data(),
		[&](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			if (m_identify_local_first)
				recv_identity();
			else
				recv_seed();

		});

}

void async_authenticate::recv_identity(

)
{
	ptr<std::vector<uint8_t>> l_data = new std::vector<uint8_t>();

	m_socket_io_guard.async_receive(*l_data,
		[&, l_data](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			byte_buffer l_byte_buffer(*l_data);

			std::vector<uint8_t> l_exported_remote_identity;

			// Try to pop the remote identity data from the byte buffer.
			if (!l_byte_buffer.pop_front(l_exported_remote_identity))
			{
				m_callback(false);
				return;
			}

			// Try to import the remote identity.
			if (!affix_base::cryptography::rsa_try_import(m_remote_public_key, l_exported_remote_identity))
			{
				m_callback(false);
				return;
			}			

			if (m_identify_local_first)
				send_seed();
			else
				send_identity();

		});

}

void async_authenticate::send_seed(

)
{
	byte_buffer l_byte_buffer;

	// Attempt to push remote seed onto byte buffer for the remote peer
	if (!l_byte_buffer.push_back(m_remote_seed))
	{
		m_callback(false);
		return;
	}

	// Try to encrypt the data with the remote peer's public key
	std::vector<uint8_t> l_encrypted_data;
	if (!affix_base::cryptography::rsa_try_encrypt_in_chunks(l_byte_buffer.data(), m_remote_public_key, l_encrypted_data))
	{
		m_callback(false);
		return;
	}

	m_socket_io_guard.async_send(l_encrypted_data,
		[&](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			if (m_identify_local_first)
				recv_seed();
			else
				recv_auth();

		});

}

void async_authenticate::recv_seed(

)
{
	ptr<std::vector<uint8_t>> l_data = new std::vector<uint8_t>();

	m_socket_io_guard.async_receive(*l_data,
		[&, l_data](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			// Try to decrypt the received data using our private key.
			std::vector<uint8_t> l_decrypted_data;
			if (!affix_base::cryptography::rsa_try_decrypt_in_chunks(*l_data, m_local_key_pair.private_key, l_decrypted_data))
			{
				m_callback(false);
				return;
			}

			byte_buffer l_byte_buffer(l_decrypted_data);

			// Try to pop the local seed off of the byte buffer
			if (!l_byte_buffer.pop_front(m_local_seed))
			{
				m_callback(false);
				return;
			}

			// Check if the seeds are the same size
			if (m_local_seed.size() != m_remote_seed.size())
			{
				m_callback(false);
				return;
			}

			if (m_identify_local_first)
				send_auth();
			else
				send_seed();

		});

}

void async_authenticate::send_auth(

)
{
	byte_buffer l_byte_buffer;

	// Try to push the LOCAL seed onto the byte buffer (for proof that you were able to decipher it)
	if (!l_byte_buffer.push_back(m_local_seed))
	{
		m_callback(false);
		return;
	}

	// Try to encrypt the byte buffer
	std::vector<uint8_t> l_encrypted_data;
	if (!affix_base::cryptography::rsa_try_encrypt_in_chunks(l_byte_buffer.data(), m_remote_public_key, l_encrypted_data))
	{
		m_callback(false);
		return;
	}

	m_socket_io_guard.async_send(l_encrypted_data,
		[&](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			if (m_identify_local_first)
				recv_auth();
			else
				m_callback(true);

		});

}

void async_authenticate::recv_auth(

)
{
	ptr<std::vector<uint8_t>> l_data = new std::vector<uint8_t>();

	m_socket_io_guard.async_receive(*l_data,
		[&, l_data](bool a_result)
		{
			if (!a_result)
			{
				m_callback(false);
				return;
			}

			// Try to decrypt the received data using our private key.
			std::vector<uint8_t> l_decrypted_data;
			if (!affix_base::cryptography::rsa_try_decrypt_in_chunks(*l_data, m_local_key_pair.private_key, l_decrypted_data))
			{
				m_callback(false);
				return;
			}

			byte_buffer l_byte_buffer(l_decrypted_data);

			// The authentication vector (just consists of the exact same seed which we just sent them)
			std::vector<uint8_t> l_remote_seed;

			if (!l_byte_buffer.pop_front(l_remote_seed))
			{
				m_callback(false);
				return;
			}

			// Verify that the seed they responded to us with is EXACTLY what we seeded them with in the first place
			if (!std::equal(l_remote_seed.begin(), l_remote_seed.end(), m_remote_seed.begin(), m_remote_seed.end()))
			{
				m_callback(false);
				return;
			}

			if (m_identify_local_first)
				m_callback(true);
			else
				send_auth();

		});

}
