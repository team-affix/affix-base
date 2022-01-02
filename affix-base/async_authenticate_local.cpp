#include "pch.h"
#include "async_authenticate_local.h"

using namespace affix_base;
using networking::async_authenticate_local;
using affix_base::data::byte_buffer;
using affix_base::data::ptr;
using std::vector;

async_authenticate_local::async_authenticate_local(
	socket_io_guard& a_socket_io_guard,
	affix_base::cryptography::rsa_key_pair a_local_key_pair,
	const std::function<void(bool)>& a_callback
) :
	m_socket_io_guard(a_socket_io_guard),
	m_local_key_pair(a_local_key_pair),
	m_callback(a_callback)
{
	async_recv_seed();
}

void async_authenticate_local::async_recv_seed()
{

}

void async_authenticate_local::async_send_auth()
{

}
