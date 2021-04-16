#pragma once
#include "node.h"
#include <functional>
#include <thread>

using affix_base::data::node;
using std::function;
using std::thread;

namespace affix_base {
	namespace callback {
		template<class T>
		class watch {
		public:
			~watch() {
				stop_thread();
			}
			watch() {

			}
			watch(T& a_var, function<void(T&)> a_on_update) {
				m_ptr = &a_var;
				m_on_update = a_on_update;
				reset_val();
			}

		public:
			void start() {
				s_watches.insert(0, *this);
			}
			void stop() {

			}

		public:
			static void start_thread() {
				m_continue = true;
				s_thd = new thread([&]() {
					while (m_continue)
						run_checks();
				});
			}
			static void stop_thread() {
				if (s_thd == nullptr)
					return;
				m_continue = false;
				s_thd->join();
			}

		private:
			void reset_val() {
				m_val = *m_ptr;
			}
			void check() {
				if (*m_ptr != m_val) {
					m_on_update(*m_ptr);
					m_val = *m_ptr;
				}
			}

		private:
			static void run_checks() {
				node<watch<T>>* l_node = &s_watches;
				for (int i = 0; l_node != nullptr; i++) {
					watch<T>& l_watch = l_node->val();
					l_watch.check();
					l_node = &l_node->fwd(1);
				}
			}

		private:
			T* m_ptr;
			T m_val;
			function<void(T&)> m_on_update;

		private:
			static bool m_continue;
			static node<watch<T>> s_watches;
			static ptr<thread> s_thd = nullptr;

		};

	}
}

