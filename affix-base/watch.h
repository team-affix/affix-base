#pragma once
#include "pch.h"

namespace affix_base {
	namespace callback {

		class watch {
		protected:
			std::function<bool()> m_predicate;
			std::function<void()> m_callback;
			bool m_predicate_match = false;

		public:
			watch(const std::function<bool()>& a_predicate, const std::function<void()>& a_callback);

		public:
			void process();

		};

	}
}
