#include "pch.h"
#include "transmission.h"
#include "serialize.h"

using namespace affix_base;

const size_t MAX_BUFFER_SIZE = 128;

bool networking::socket_send(tcp::socket& a_socket, const vector<uint8_t>& a_data) {
	if (!socket_send_size(a_socket, a_data.size()))
		return false;
	if (!socket_send_data(a_socket, a_data))
		return false;
	return true;
}
bool networking::socket_send_size(tcp::socket& a_socket, const uint32_t& a_size) {
	return socket_send_data(a_socket, serialize(a_size));
}
bool networking::socket_send_data(tcp::socket& a_socket, const vector<uint8_t>& a_data) {
	
	asio::error_code l_ec;
	size_t l_offset = 0;

	while (l_offset < a_data.size()) {

		size_t l_remaining = a_data.size() - l_offset;
		l_offset += a_socket.write_some(asio::buffer(a_data.data() + l_offset, std::min(l_remaining, MAX_BUFFER_SIZE)), l_ec);

		if (l_ec) {
			return false;
		}

	}

	return true;

}

bool networking::socket_receive(tcp::socket& a_socket, vector<uint8_t>& a_data) {
	uint32_t l_size = 0;
	if (!socket_receive_size(a_socket, l_size))
		return false;
	a_data.resize(l_size);
	if (!socket_receive_data(a_socket, a_data))
		return false;
	return true;
}
bool networking::socket_receive_size(tcp::socket& a_socket, uint32_t& a_size) {
	vector<uint8_t> l_size_vector(sizeof(uint32_t));
	if (!socket_receive_data(a_socket, l_size_vector))
		return false;
	a_size = deserialize<uint32_t>(l_size_vector);
	return true;
}
bool networking::socket_receive_data(tcp::socket& a_socket, vector<uint8_t>& a_data) {

	asio::error_code l_ec;
	size_t l_offset = 0;

	while (l_offset < a_data.size()) {

		size_t l_remaining = a_data.size() - l_offset;
		l_offset += a_socket.read_some(asio::mutable_buffer(a_data.data() + l_offset, std::min(l_remaining, MAX_BUFFER_SIZE)), l_ec);

		if (l_ec) {
			LOG("[ ERROR ] " << l_ec.message());
			return false;
		}

	}

	return true;

}

void networking::socket_async_send(tcp::socket& a_socket, const vector<uint8_t>& a_data, std::function<void(bool)> a_callback) {
	socket_async_send_size(a_socket, a_data.size(), [&, a_callback](bool a_result) {
		if (!a_result) {
			a_callback(false);
			return;
		}
		socket_async_send_data(a_socket, a_data, [&, a_callback](bool a_result) {
			a_callback(a_result);
			});
		});
}
void networking::socket_async_send_size(tcp::socket& a_socket, const uint32_t& a_size, std::function<void(bool)> a_callback) {
	shared_ptr<vector<uint8_t>> l_size_vector = std::make_shared<vector<uint8_t>>(serialize(a_size));
	socket_async_send_data(a_socket, *l_size_vector, [&, l_size_vector, a_callback](bool a_result) {
		a_callback(a_result);
		});
}
void networking::socket_async_send_data(tcp::socket& a_socket, const vector<uint8_t>& a_data, std::function<void(bool)> a_callback, size_t a_offset) {
	size_t l_remaining = a_data.size() - a_offset;
	if (l_remaining > 0) {
		a_socket.async_write_some(asio::const_buffer(a_data.data() + a_offset, std::min(l_remaining, MAX_BUFFER_SIZE)), [&, a_callback, a_offset](error_code a_ec, size_t a_bytes_sent) {
			size_t l_new_offset = a_offset + a_bytes_sent;
			if (!a_ec) {
				socket_async_send_data(a_socket, a_data, a_callback, l_new_offset);
			}
			else {
				a_callback(false);
			}
		});
	}
	else {
		a_callback(true);
	}
}

void networking::socket_async_receive(tcp::socket& a_socket, vector<uint8_t>& a_data, std::function<void(bool)> a_callback) {
	shared_ptr<uint32_t> l_size = std::make_shared<uint32_t>(0);
	socket_async_receive_size(a_socket, *l_size, [&,a_callback,l_size](bool a_result) {
		if (!a_result) {
			a_callback(false);
			return;
		}
		a_data.resize(*l_size);
		socket_async_receive_data(a_socket, a_data, a_callback);
	});
}
void networking::socket_async_receive_size(tcp::socket& a_socket, uint32_t& a_size, std::function<void(bool)> a_callback) {
	shared_ptr<vector<uint8_t>> l_size_vector = shared_ptr<vector<uint8_t>>(new vector<uint8_t>(sizeof(uint32_t)));
	socket_async_receive_data(a_socket, *l_size_vector, [&,l_size_vector,a_callback] (bool a_result) {
		if (!a_result) {
			a_callback(false);
			return;
		}
		a_size = deserialize<uint32_t>(*l_size_vector);
		a_callback(true);
	});
}
void networking::socket_async_receive_data(tcp::socket& a_socket, vector<uint8_t>& a_data, std::function<void(bool)> a_callback, size_t a_offset) {
	size_t l_remaining = a_data.size() - a_offset;
	if (l_remaining > 0) {
		a_socket.async_read_some(asio::buffer(a_data.data() + a_offset, std::min(l_remaining, MAX_BUFFER_SIZE)), [&,a_callback,a_offset] (error_code a_ec, size_t a_bytes_received) {
			size_t l_new_offset = a_offset + a_bytes_received;
			if (!a_ec) {
				socket_async_receive_data(a_socket, a_data, a_callback, l_new_offset);
			}
			else {
				a_callback(false);
			}
		});
	}
	else {
		a_callback(true);
	}
}
