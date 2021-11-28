#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {

		template<typename T>
		struct diff {
		public:
			T val;
			bool addition = false;
		};

	}
}
