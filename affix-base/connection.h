#pragma once
#include "pch.h"
#include "asio.hpp"
#include "message.h"
#include "rolling_buffer.h"
#include "ts_deque.h"
#include <iostream>

namespace affix_base {
	namespace networking {

		using asio::io_context;
		using asio::ip::tcp;
		using affix_base::data::builder;
		using affix_base::data::rolling_buffer;
		using std::string;
		using affix_base::data::ts_deque;
		using namespace asio;
	
		class connection : public std::enable_shared_from_this<connection> {
		public:
			const vector<uint8_t> MESSAGE_INCOMING = { 'M', 'E', 'S', 'S', 'A', 'G', 'E' };

		protected:
			tcp::socket m_socket;
			vector<uint8_t> m_buffer_received;

		protected:
			rolling_buffer<uint8_t> m_rolling_buffer;

		protected:
			bool m_building = false;
			builder m_size_builder;
			builder m_data_builder;

		public:
			uint32_t m_id = 0;
			std::function<void(connection&, vector<uint8_t>&)> m_process_inbound_data;
			std::function<void(connection&, vector<uint8_t>&)> m_process_outbound_data;
			std::function<void(connection&)> m_on_data_sent;
			std::function<void(connection&)> m_on_local_disconnect;
			std::function<void(connection&)> m_on_remote_disconnect;

		public:
			virtual ~connection() {
				m_socket.close();
			}
			connection(uint32_t a_id, tcp::socket& a_socket, uint32_t a_max_buffer_size) : m_id(a_id), m_socket(std::move(a_socket)), m_buffer_received(a_max_buffer_size), m_rolling_buffer(MESSAGE_INCOMING.size()) {
	#if NET_COMMON_DEBUG
				std::cout << "[ CONNECTION ] Connection object created." << std::endl;
	#endif
			}

		public:
			void async_send(vector<uint8_t> a_data_vector) {

				if (m_process_outbound_data != nullptr)
					m_process_outbound_data(*this, a_data_vector);

				uint32_t l_data_size = a_data_vector.size();
				vector<uint8_t> l_data_size_vector(sizeof(uint32_t));
				memcpy(l_data_size_vector.data(), &l_data_size, sizeof(uint32_t));
			
				vector<uint8_t> l_full_data;
				l_full_data.insert(l_full_data.end(), MESSAGE_INCOMING.begin(), MESSAGE_INCOMING.end());
				l_full_data.insert(l_full_data.end(), l_data_size_vector.begin(), l_data_size_vector.end());
				l_full_data.insert(l_full_data.end(), a_data_vector.begin(), a_data_vector.end());

				m_socket.async_write_some(asio::buffer(l_full_data.data(), l_full_data.size()),
					[&](error_code a_ec, size_t a_length) {
						if (remote_disconnected(a_ec))
							return;
	#if NET_COMMON_DEBUG
					std::cout << "[ CONNECTION ] Sent data; size: " << a_length << std::endl;
	#endif
					if (m_on_data_sent != nullptr)
						m_on_data_sent(*this);
				});

			}
			void async_receive() {
				m_socket.async_read_some(asio::buffer(m_buffer_received.data(), m_buffer_received.size()),
					[&](asio::error_code l_ec, size_t l_length) {

						if (local_disconnected(l_ec)) {
	#if NET_COMMON_DEBUG
							std::cout << "[ CONNECTION ] Local disconnect. " << std::endl;
	#endif
							if (m_on_local_disconnect != nullptr)
								m_on_local_disconnect(*this);
							return;
						}
						if (remote_disconnected(l_ec)) {
	#if NET_COMMON_DEBUG
							std::cout << "[ CONNECTION ] Remote disconnect. " << std::endl;
	#endif
							if (m_on_remote_disconnect != nullptr)
								m_on_remote_disconnect(*this);
							return;
						}

	#if NET_COMMON_DEBUG
						std::cout << "[ CONNECTION ] Received bytes: " << l_length << std::endl;
	#endif

						for (int i = 0; i < l_length; i++)
							process_byte(m_buffer_received[i]);

						async_receive();

					});
			}

		protected:
			bool remote_disconnected(const asio::error_code& a_ec) {
				return a_ec == asio::error::eof
					|| a_ec == asio::error::connection_reset;
			}
			bool local_disconnected(const asio::error_code& a_ec) {
				return a_ec == asio::error::connection_aborted
				|| a_ec.value() == 1236; // local disconnect from socket
			}

		protected:
			bool message_incoming() {
				return std::equal(m_rolling_buffer.buffer().begin(), m_rolling_buffer.buffer().end(), MESSAGE_INCOMING.begin(), MESSAGE_INCOMING.end());
			}
			void process_byte(const uint8_t& a_byte) {

				if (!m_building) {
					m_rolling_buffer << a_byte;
					if (message_incoming()) {
	#if NET_COMMON_DEBUG
						std::cout << "[ CONNECTION ] Data incoming." << std::endl;
	#endif
						m_building = true;
						m_size_builder = builder(sizeof(uint32_t));
					}
				}
				else if (!m_size_builder.ready()) {
					if (m_size_builder << a_byte) {
						uint32_t l_body_size = m_size_builder.build<uint32_t>();
						m_data_builder = builder(l_body_size);
					}
				}
				else if (!m_data_builder.ready()) {
					if (m_data_builder << a_byte) {
						// BUILDER HAS COLLECTED ALL RELEVANT DATA
	#if NET_COMMON_DEBUG
						std::cout << "[ CONNECTION ] Received data." << std::endl;
	#endif
						if (m_process_inbound_data != nullptr)
							m_process_inbound_data(*this, m_data_builder.buffer());

						m_building = false;
					}
				}
			}

		public:
			tcp::socket& socket() {
				return m_socket;
			}

		};

	}
}
