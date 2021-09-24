#pragma once
#include "pch.h"
#include "asio.hpp"
#include "byte_buffer.h"
#include "rolling_buffer.h"
#include "ts_deque.h"
#include "ptr.h"
#include <iostream>

#if 1
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif

namespace affix_base {
	namespace networking {

		using asio::io_context;
		using asio::ip::tcp;
		using asio::ip::udp;
		using affix_base::data::builder;
		using affix_base::data::rolling_buffer;
		using std::string;
		using affix_base::data::ts_deque;
		using std::shared_ptr;
		using namespace affix_base::data;
		using namespace asio;

		// TCP SYNCHRONOUS SEND
		bool socket_send(tcp::socket& a_socket, const vector<uint8_t>& a_data);
		bool socket_send_size(tcp::socket& a_socket, const uint32_t& a_size);
		bool socket_send_data(tcp::socket& a_socket, const vector<uint8_t>& a_data);

		// TCP SYNCHRONOUS RECEIVE
		bool socket_receive(tcp::socket& a_socket, vector<uint8_t>& a_data);
		bool socket_receive_size(tcp::socket& a_socket, uint32_t& a_size);
		bool socket_receive_data(tcp::socket& a_socket, vector<uint8_t>& a_data);

		// TCP ASYNCHRONOUS SEND
		void socket_async_send(tcp::socket& a_socket, const vector<uint8_t>& a_data, std::function<void(bool)> a_callback);
		void socket_async_send_size(tcp::socket& a_socket, const uint32_t& a_size, std::function<void(bool)> a_callback);
		void socket_async_send_data(tcp::socket& a_socket, const vector<uint8_t>& a_data, std::function<void(bool)> a_callback, size_t a_offset = 0);

		// TCP ASYNCHRONOUS RECEIVE
		void socket_async_receive(tcp::socket& a_socket, vector<uint8_t>& a_data, std::function<void(bool)> a_callback);
		void socket_async_receive_size(tcp::socket& a_socket, uint32_t& a_size, std::function<void(bool)> a_callback);
		void socket_async_receive_data(tcp::socket& a_socket, vector<uint8_t>& a_data, std::function<void(bool)> a_callback, size_t a_offset = 0);

		// UDP SYNCHRONOUS SEND
		bool socket_send_data_to(udp::socket& a_socket, const udp::endpoint& a_remote_endpoint, const vector<uint8_t>& a_data);
		
		// UDP SYNCHRONOUS RECEIVE
		bool socket_receive_data_for(udp::socket& a_socket, vector<uint8_t>& a_data, const size_t& a_seconds_to_receive);

	}
}
