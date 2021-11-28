#pragma once
#include "pch.h"
#include "ptr.h"

namespace affix_base {
	namespace threading {
		class persistent_thread {
		protected:
			std::thread m_thread;

		public:
			virtual ~persistent_thread();
			persistent_thread();
			persistent_thread(std::function<void()> a_func);
			persistent_thread(const persistent_thread& a_other);
			void operator=(const persistent_thread& a_other);

		private:
			void init();

		public:
			void call();
			void call(std::function<void()> a_func);
			void loop();
			void loop(std::function<void()> a_func);
			void stop_loop();
			void join_call();
			void join_loop();

		public:
			affix_base::data::ptr<bool> m_persist = new bool(true);
			affix_base::data::ptr<bool> m_call = new bool(false);
			affix_base::data::ptr<bool> m_loop = new bool(false);
			affix_base::data::ptr<bool> m_executing = new bool(false);
			std::function<void()> m_function;

		};
	}
}