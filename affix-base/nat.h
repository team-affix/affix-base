#pragma once
#include "pch.h"
#include "message.h"
#include "asio.hpp"
#include "cryptopp/rsa.h"

namespace affix_base {
	namespace networking {

		using namespace asio;
		using namespace asio::ip;
		using asio::io_context;
		using CryptoPP::RSA;
		
		enum class nat_type {
			symmetric,
			non_symmetric
		};

		bool socket_external_ip(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, const RSA::PublicKey& a_returner_public_key, tcp::endpoint& a_output);
		bool socket_nat_type(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, nat_type& a_output);

	}
}
