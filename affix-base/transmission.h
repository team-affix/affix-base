#pragma once
#include "pch.h"
#include "asio.hpp"
#include "byte_buffer.h"
#include "rolling_buffer.h"
#include "ts_deque.h"
#include "ptr.h"
#include <iostream>

namespace affix_base {
	namespace networking {

		// TCP SYNCHRONOUS SEND
		bool socket_send(
			asio::ip::tcp::socket& a_socket,
			const std::vector<uint8_t>& a_data);

		bool socket_send_size(
			asio::ip::tcp::socket& a_socket,
			const uint32_t& a_size);

		bool socket_send_data(
			asio::ip::tcp::socket& a_socket,
			const std::vector<uint8_t>& a_data);

		// TCP SYNCHRONOUS RECEIVE
		bool socket_receive(
			asio::ip::tcp::socket& a_socket,
			std::vector<uint8_t>& a_data);

		bool socket_receive_size(
			asio::ip::tcp::socket& a_socket,
			uint32_t& a_size);

		bool socket_receive_data(
			asio::ip::tcp::socket& a_socket,
			std::vector<uint8_t>& a_data);

		// TCP ASYNCHRONOUS SEND
		void socket_async_send(
			asio::ip::tcp::socket& a_socket,
			const std::vector<uint8_t>& a_data,
			std::function<void(bool)> a_callback);

		void socket_async_send_size(
			asio::ip::tcp::socket& a_socket,
			const uint32_t& a_size,
			std::function<void(bool)> a_callback);

		void socket_async_send_data(
			asio::ip::tcp::socket& a_socket,
			const std::vector<uint8_t>& a_data,
			std::function<void(bool)> a_callback, size_t a_offset = 0);

		// TCP ASYNCHRONOUS RECEIVE
		void socket_async_receive(
			asio::ip::tcp::socket& a_socket,
			std::vector<uint8_t>& a_data,
			std::function<void(bool)> a_callback);

		void socket_async_receive_size(
			asio::ip::tcp::socket& a_socket,
			uint32_t& a_size,
			std::function<void(bool)> a_callback);

		void socket_async_receive_data(
			asio::ip::tcp::socket& a_socket,
			std::vector<uint8_t>& a_data,
			std::function<void(bool)> a_callback,
			size_t a_offset = 0);

		// UDP SYNCHRONOUS SEND
		bool socket_send_data_to(
			asio::ip::udp::socket& a_socket,
			const asio::ip::udp::endpoint& a_remote_endpoint,
			const std::vector<uint8_t>& a_data);
		
		// UDP SYNCHRONOUS RECEIVE
		bool socket_receive_data_for(
			asio::ip::udp::socket& a_socket,
			std::vector<uint8_t>& a_data,
			const size_t& a_seconds_to_receive);

	}
}
