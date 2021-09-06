#pragma once
#include "pch.h"
#include <chrono>

namespace affix_base {
	namespace timing {

		inline uint64_t utc_time() {
			const auto p1 = std::chrono::system_clock::now();
			return std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
		}

	}
}
