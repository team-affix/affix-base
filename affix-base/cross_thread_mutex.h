#pragma once
#include "pch.h"

namespace affix_base {
	namespace threading {

		class cross_thread_mutex {
		protected:
			std::mutex m_state_mutex;
			std::mutex m_mutex;

		protected:
			std::thread::id m_id;
			uint16_t m_lock_index = 0;

		public:
			virtual ~cross_thread_mutex();

		public:
			void lock();
			bool try_lock();
			void unlock();
			std::thread::id id();

		};

	}
}
