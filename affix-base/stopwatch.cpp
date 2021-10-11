#include "pch.h"
#include "stopwatch.h"

using namespace affix_base::timing;

void stopwatch::start() {
	m_start_time = std::chrono::high_resolution_clock::now();
}

long long stopwatch::duration_milliseconds() const {
	auto l_end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(l_end_time - m_start_time).count();
}

long long stopwatch::duration_microseconds() const {
	auto l_end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(l_end_time - m_start_time).count();
}

long long stopwatch::duration_nanoseconds() const {
	auto l_end_time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(l_end_time - m_start_time).count();
}
