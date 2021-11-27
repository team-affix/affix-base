#pragma once
#include "pch.h"

namespace affix_base {
	namespace callback {

		using std::function;

		class watch {
		protected:
			function<bool()> m_predicate;
			function<void()> m_callback;
			bool m_predicate_match = false;

		public:
			watch(const function<bool()>& a_predicate, const function<void()>& a_callback);

		public:
			void process();

		};

	}
}
