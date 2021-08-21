#pragma once
#include "pch.h"
#include "asio.hpp"
#include "message.h"
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
		using affix_base::data::builder;
		using affix_base::data::rolling_buffer;
		using std::string;
		using affix_base::data::ts_deque;
		using std::shared_ptr;
		using namespace affix_base::data;
		using namespace asio;
		
		// SYNCHRONOUS SEND
		bool socket_send(tcp::socket& a_socket, const vector<uint8_t>& a_data);
		bool socket_send_size(tcp::socket& a_socket, const uint32_t& a_size);
		bool socket_send_data(tcp::socket& a_socket, const vector<uint8_t>& a_data);

		// SYNCHRONOUS RECEIVE
		bool socket_receive(tcp::socket& a_socket, vector<uint8_t>& a_data);
		bool socket_receive_size(tcp::socket& a_socket, uint32_t& a_size);
		bool socket_receive_data(tcp::socket& a_socket, vector<uint8_t>& a_data);

		// ASYNCHRONOUS SEND
		void socket_async_send(tcp::socket& a_socket, const vector<uint8_t>& a_data, std::function<void(bool)> a_callback = nullptr);
		void socket_async_send_size(tcp::socket& a_socket, const uint32_t& a_size, std::function<void(bool)> a_callback);
		void socket_async_send_data(tcp::socket& a_socket, const vector<uint8_t>& a_data, std::function<void(bool)> a_callback, size_t a_offset = 0);

		// ASYNCHRONOUS RECEIVE
		void socket_async_receive(tcp::socket& a_socket, vector<uint8_t>& a_data, std::function<void(bool)> a_callback);
		void socket_async_receive_size(tcp::socket& a_socket, uint32_t& a_size, std::function<void(bool)> a_callback);
		void socket_async_receive_data(tcp::socket& a_socket, vector<uint8_t>& a_data, std::function<void(bool)> a_callback, size_t a_offset = 0);

	}
}
