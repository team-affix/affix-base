#pragma once
#include "pch.h"
#include "ptr.h"

namespace affix_base {
	namespace threading {
		class persistent_thread {
		protected:
			std::thread::id m_owner_id;
			std::thread m_thread;
			bool m_abort_thread = false;

		public:
			std::function<void()>* m_function = nullptr;
			volatile bool m_trigger = false;

		public:
			virtual ~persistent_thread(

			);

			persistent_thread(

			);

			void execute(
				std::function<void()>& a_function
			);

			void join(

			);

		};
	}
}