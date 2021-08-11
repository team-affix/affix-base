#pragma once
#include "pch.h"
#include <deque>

namespace affix_base {
	namespace data {

		using std::deque;

		template<typename T>
		class rolling_buffer {
		protected:
			size_t m_capacity = 0;
			deque<T> m_buffer;

		public:
			rolling_buffer(size_t a_capacity) : m_capacity(a_capacity), m_buffer(a_capacity) {
				
			}

		public:
			friend rolling_buffer<T>& operator << (rolling_buffer<T>& a_buffer, const T& a_val) {
				a_buffer.m_buffer.pop_front();
				a_buffer.m_buffer.push_back(a_val);
				return a_buffer;
			}

		public:
			const deque<T>& buffer() {
				return m_buffer;
			}

		};

	}
}
