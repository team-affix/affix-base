#pragma once
#include "pch.h"
#include "ptr.h"

using affix_base::data::ptr;
using std::thread;
using std::function;

namespace affix_base {
	namespace threading {
		class persistent_thread {
		protected:
			thread m_thread;

		public:
			~persistent_thread();
			persistent_thread();
			persistent_thread(function<void()> a_func);
			persistent_thread(const persistent_thread& a_other);
			void operator=(const persistent_thread& a_other);

		private:
			void init();

		public:
			void call();
			void call(function<void()> a_func);
			void repeat();
			void repeat(function<void()> a_func);
			void stop_repeat();
			void join_call();
			void join_repeat();

		public:
			ptr<bool> m_persist = new bool(true);
			ptr<bool> m_call = new bool(false);
			ptr<bool> m_repeat = new bool(false);
			ptr<bool> m_executing = new bool(false);
			function<void()> m_function;

		};
	}
}