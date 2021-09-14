#pragma once
#include "pch.h"
#include "transmission.h"
#include "asio.hpp"

namespace affix_base {
	namespace networking {

        using std::vector;
        using std::function;
        using std::mutex;
        using std::lock_guard;

        struct socket_send_request {
            vector<uint8_t> m_data;
            function<void(bool)> m_callback;
        };
        struct socket_receive_request {
            vector<uint8_t>& m_data;
            function<void(bool)> m_callback;
        };

        class socket_io_guard {
        protected:
            tcp::socket& m_socket;

        protected:
            mutex m_send_mutex;
            deque<socket_send_request> m_send_deque;
            mutex m_receive_mutex;
            deque<socket_receive_request> m_receive_deque;

        public:
            socket_io_guard(tcp::socket& a_socket);

        public:
            void async_send(const vector<uint8_t>& a_data, function<void(bool)> a_callback);
            void async_receive(vector<uint8_t>& a_data, function<void(bool)> a_callback);

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
