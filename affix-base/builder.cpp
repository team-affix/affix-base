#include "pch.h"
#include "builder.h"

using affix_base::data::builder;
using std::vector;

builder::builder() {

}

builder::builder(size_t a_capacity) : m_capacity(a_capacity), m_buffer(a_capacity) {

}

bool builder::ready() const {
	return m_buffer_used_size == m_capacity;
}

size_t builder::capacity() {
	return m_capacity;
}

uint8_t* builder::data() {
	return m_buffer.data();
}

vector<uint8_t>& builder::buffer() {
	return m_buffer;
}
