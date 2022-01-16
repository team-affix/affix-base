#include "pch.h"
#include "socket_io_guard.h"
#include "cross_thread_mutex.h"

using namespace affix_base::networking;
using namespace asio;
using namespace asio::ip;
using std::vector;
using std::function;
using std::lock_guard;
using std::mutex;
using affix_base::threading::cross_thread_mutex;

socket_io_guard::socket_io_guard(tcp::socket& a_socket) : m_socket(a_socket) {

}

void socket_io_guard::async_send(const vector<uint8_t>& a_data, function<void(bool)> a_callback) {

    lock_guard<cross_thread_mutex> l_lock(m_send_mutex);
    m_send_deque.push_back({ a_data, a_callback });
    if (m_send_deque.size() == 1)
        send_next();

}

void socket_io_guard::async_receive(vector<uint8_t>& a_data, function<void(bool)> a_callback) {

    lock_guard<cross_thread_mutex> l_lock(m_receive_mutex);
    m_receive_deque.push_back({ a_data, a_callback });
    if (m_receive_deque.size() == 1)
        receive_next();

}

void socket_io_guard::send_callback(const bool& a_result) {
    
    lock_guard<cross_thread_mutex> l_lock(m_send_mutex);
    m_send_deque.front().m_callback(a_result);
    m_send_deque.pop_front();
    if (m_send_deque.size() > 0 && a_result)
        send_next();

}

void socket_io_guard::receive_callback(const bool& a_result) {

    lock_guard<cross_thread_mutex> l_lock(m_receive_mutex);
    m_receive_deque.front().m_callback(a_result);
    m_receive_deque.pop_front();
    if (m_receive_deque.size() > 0 && a_result)
        receive_next();

}

void socket_io_guard::send_next() {

    socket_async_send(m_socket, m_send_deque.front().m_data, 
        [&](const bool& a_result)
        { 
            try
            {
                send_callback(a_result);
            }
            catch (...)
            {
                std::cerr << "[ SOCKET IO GUARD ] Error: unable to successfully complete the send callback." << std::endl;
            }
        });

}

void socket_io_guard::receive_next() {

    socket_async_receive(m_socket, m_receive_deque.front().m_data,
        [&](const bool& a_result)
        { 
            try
            {
                receive_callback(a_result);
            }
            catch (...)
            {
                std::cerr << "[ SOCKET IO GUARD ] Error: unable to successfully complete the receive callback." << std::endl;
            }
        });

}
