#pragma once
#include "pch.h"
#include "byte_buffer.h"
#include "asio.hpp"
#include "cryptopp/rsa.h"

namespace affix_base {
	namespace networking {

		enum class nat_type {
			unknown,
			symmetric,
			non_symmetric
		};

		void socket_reopen_and_rebind(
			asio::ip::udp::socket& a_socket);

		bool socket_external_ip(
			asio::ip::udp::socket& a_socket,
			const asio::ip::udp::endpoint& a_returner_endpoint,
			const CryptoPP::RSA::PublicKey & a_returner_public_key,
			asio::ip::udp::endpoint& a_output);

		bool socket_external_ip(
			asio::ip::udp::socket& a_socket,
			const asio::ip::udp::endpoint& a_returner_endpoint,
			const CryptoPP::RSA::PublicKey& a_returner_public_key,
			asio::ip::udp::endpoint& a_output,
			const size_t& a_max_attempts);

		bool socket_nat_type(
			asio::ip::udp::socket& a_socket,
			const asio::ip::udp::endpoint& a_returner_endpoint_0,
			const CryptoPP::RSA::PublicKey& a_returner_public_key_0,
			const asio::ip::udp::endpoint& a_returner_endpoint_1,
			const CryptoPP::RSA::PublicKey& a_returner_public_key_1,
			const size_t& a_max_attempts, nat_type& a_output);

		bool socket_nat_type(
			asio::ip::udp::socket& a_socket,
			const asio::ip::udp::endpoint& a_returner_endpoint_0,
			const CryptoPP::RSA::PublicKey& a_returner_public_key_0,
			const asio::ip::udp::endpoint& a_returner_endpoint_1,
			const CryptoPP::RSA::PublicKey& a_returner_public_key_1,
			const size_t& a_max_attempts,
			nat_type& a_output,
			const size_t& a_trials);


	}
}
