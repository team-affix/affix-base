#pragma once
#include "pch.h"
#include <chrono>

namespace affix_base {
	namespace timing {

		class stopwatch {
		protected:
			std::chrono::steady_clock::time_point m_start_time;
			std::chrono::steady_clock::time_point m_stop_time;
			
		public:
			void start();
			long long duration_milliseconds() const;
			long long duration_microseconds() const;
			long long duration_nanoseconds() const;

		};

	}
}
