#pragma once
#include "pch.h"
#include "persistent_thread.h"

namespace affix_base {
	namespace threading {
		class persistent_thread_group : public vector<persistent_thread> {
		protected:
			ptr<bool> m_execute_start = new bool(false);

		public:
			persistent_thread_group();
			persistent_thread_group(vector<persistent_thread> a_vec);

		public:
			void compile();
			void execute();
			void join();

		};
	}
}