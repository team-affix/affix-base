#pragma once
#include "message.h"
#include "asio.hpp"

namespace affix_base {
	namespace networking {

		using namespace asio;
		using namespace asio::ip;
		using asio::io_context;
		
		enum class nat_type {
			symmetric,
			non_symmetric
		};

		bool get_external_ip(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, tcp::endpoint& a_output);
		bool get_nat_type(tcp::socket& a_socket, tcp::endpoint a_returner_endpoint, nat_type& a_output);

	}
}
