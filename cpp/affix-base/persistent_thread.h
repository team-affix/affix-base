#pragma once
#include <thread>
#include <functional>

using std::thread;
using std::function;

namespace affix {
	namespace threading {
		class persistent_thread {
		public:
			~persistent_thread();
			persistent_thread();
			persistent_thread(function<void()> a_func);

		private:
			void init();

		public:
			void execute();
			void execute(function<void()> a_func);

			void join();

		public:
			function<void()> m_function;

		private:
			volatile bool m_continue = true;
			volatile bool m_executing = false;
			thread m_thread;

		};
	}
}