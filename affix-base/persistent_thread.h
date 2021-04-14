#pragma once
#include "ptr.h"
#include <thread>
#include <functional>

using affix::data::ptr;
using std::thread;
using std::function;

namespace affix {
	namespace threading {
		class persistent_thread {
		public:
			~persistent_thread();
			persistent_thread();
			persistent_thread(function<void()> a_func);
			persistent_thread(const persistent_thread& a_other);
			void operator=(const persistent_thread& a_other);

		private:
			void init();

		public:
			void execute();
			void execute(function<void()> a_func);
			void join();

		public:
			ptr<bool> m_continue = new bool(true);
			ptr<bool> m_execute_start = new bool(false);
			ptr<bool> m_executing = new bool(false);
			function<void()> m_function;

		private:
			thread m_thread;

		};
	}
}