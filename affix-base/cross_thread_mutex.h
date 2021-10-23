#pragma once
#include "pch.h"

namespace affix_base {
	namespace threading {

		using std::mutex;

		class cross_thread_mutex {
		protected:
			mutex m_state_mutex;
			mutex m_mutex;

		protected:
			std::thread::id m_id;
			uint16_t m_lock_index = 0;

		public:
			void lock();
			void unlock();

		};

	}
}