#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {
		template<typename T>
		class deque_mt {
		protected:
			std::mutex m_queue_mtx;
			std::deque<T> m_deque;

		public:
			virtual ~queue_mt() { clear(); }
			queue_mt() = default;
			queue_mt(const queue_mt<T>&) = delete;

		public:
			const T& front() {
				std::scoped_lock lock(m_deque);
				return m_deque.front();
			}
			const T& back() {
				std::scoped_lock lock(m_deque);
				return m_deque.back();
			}
			void push_back(const T& item) {
				std::scoped_lock lock(m_deque);
				m_deque.emplace_back(std::move(item));
			}
			void push_front(const T& item) {
				std::scoped_lock lock(m_deque);
				m_deque.emplace_front(std::move(item));
			}
			size_t count() {
				std::scoped_lock lock(m_deque);
				return m_deque.size();
			}
			void clear() {
				std::scoped_lock lock(m_deque);
				m_deque.clear();
			}
			T pop_front() {
				std::scoped_lock lock(m_deque);
				auto result = m_deque.front();
				m_deque.pop_front();
				return result;
			}
			T pop_back() {
				std::scoped_lock lock(m_deque);
				auto result = m_deque.back();
				m_deque.pop_back();
				return result;
			}

		};
	}
}