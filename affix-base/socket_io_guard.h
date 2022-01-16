#pragma once
#include "pch.h"
#include "transmission.h"
#include "asio.hpp"
#include "cross_thread_mutex.h"

namespace affix_base {
	namespace networking {

        struct socket_send_request {
            std::vector<uint8_t> m_data;
            std::function<void(bool)> m_callback;
        };
        struct socket_receive_request {
            std::vector<uint8_t>& m_data;
            std::function<void(bool)> m_callback;
        };

        class socket_io_guard {
        protected:
            asio::ip::tcp::socket& m_socket;

        protected:
            affix_base::threading::cross_thread_mutex m_send_mutex;
            std::deque<socket_send_request> m_send_deque;
            affix_base::threading::cross_thread_mutex m_receive_mutex;
            std::deque<socket_receive_request> m_receive_deque;

        public:
            socket_io_guard(
                asio::ip::tcp::socket& a_socket
            );

        public:
            void async_send(const std::vector<uint8_t>& a_data, std::function<void(bool)> a_callback);
            void async_receive(std::vector<uint8_t>& a_data, std::function<void(bool)> a_callback);

        protected:
            void send_callback(const bool& a_result);
            void receive_callback(const bool& a_result);

        protected:
            // UNPROTECTED
            void send_next();
            void receive_next();

        };

	}
}
