#pragma once
#include "persistent_thread.h"
#include <vector>

using affix_base::threading::persistent_thread;
using std::vector;

namespace affix_base {
	namespace threading {
		class persistent_thread_group : public vector<persistent_thread> {
		public:
			persistent_thread_group();
			persistent_thread_group(vector<persistent_thread> a_vec);

		public:
			void compile();
			void execute();
			void join();

		private:
			ptr<bool> m_execute_start = new bool(false);

		};
	}
}