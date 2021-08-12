#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {
		using std::deque;
		using std::mutex;

		template<typename T>
		class ts_deque {
		protected:
			deque<T> m_deque;
			mutex m_mutex;

		public:
			const size_t size() const {
				std::lock_guard l_lock(m_mutex);
				return m_deque.size();
			}
			void push_back(const T& a_val) {
				std::lock_guard l_lock(m_mutex);
				m_deque.push_back(a_val);
			}
			void push_front(const T& a_val) {
				std::lock_guard l_lock(m_mutex);
				m_deque.push_front(a_val);
			}
			void move_to_back(T& a_val) {
				push_back(std::move(a_val));
			}
			void move_to_front(T& a_val) {
				push_front(std::move(a_val));
			}
			T pop_back() {
				std::lock_guard l_lock(m_mutex);
				T result = m_deque.back();
				m_deque.pop_back();
				return result;
			}
			T pop_front() {
				std::lock_guard l_lock(m_mutex);
				T result = m_deque.front();
				m_deque.pop_front();
				return result;
			}
		};
	}
}
