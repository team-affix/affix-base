#pragma once
#include <functional>
#include "persistent_thread.h"

using std::function;

namespace affix {
	namespace callback {
		template<class T>
		class monitor {
			
		public:
			monitor() {}
			monitor(T a_T, function<void(T&)> a_predicate) : m_T(a_T), m_predicate(a_predicate) {}

		public:
			function<void(T&)> m_predicate;

		private:
			T m_T;

		};
	}
}