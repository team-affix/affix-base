#pragma once
#include "pch.h"
#include "persistent_thread.h"

namespace affix_base {
	namespace threading {

		class persistent_thread_group : public std::vector<persistent_thread> {
		protected:
			affix_base::data::ptr<bool> m_execute_start = new bool(false);

		public:
			persistent_thread_group();
			persistent_thread_group(std::vector<persistent_thread> a_vec);

		public:
			void compile();
			void execute();
			void join();

		};

	}
}