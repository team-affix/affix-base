#pragma once
#include "pch.h"
#include "byte_buffer.h"
#include "asio.hpp"
#include "cryptopp/rsa.h"

namespace affix_base {
	namespace networking {

		using namespace asio;
		using namespace asio::ip;
		using asio::io_context;
		using CryptoPP::RSA;
		
		enum class nat_type {
			unknown,
			symmetric,
			non_symmetric
		};

		void socket_reopen_and_rebind(udp::socket& a_socket);
		bool socket_external_ip(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint, const RSA::PublicKey& a_returner_public_key, udp::endpoint& a_output);
		bool socket_external_ip(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint, const RSA::PublicKey& a_returner_public_key, udp::endpoint& a_output, const size_t& a_max_attempts);
		bool socket_nat_type(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint_0, const RSA::PublicKey& a_returner_public_key_0, const udp::endpoint& a_returner_endpoint_1, const RSA::PublicKey& a_returner_public_key_1, const size_t& a_max_attempts, nat_type& a_output);
		bool socket_nat_type(udp::socket& a_socket, const udp::endpoint& a_returner_endpoint_0, const RSA::PublicKey& a_returner_public_key_0, const udp::endpoint& a_returner_endpoint_1, const RSA::PublicKey& a_returner_public_key_1, const size_t& a_max_attempts, nat_type& a_output, const size_t& a_trials);

	}
}
