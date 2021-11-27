#pragma once
#include "pch.h"
#include <string>

#define SEVERITY_UNKNOWN_STR "UNKNOWN"
#define SEVERITY_SUCCESS_STR "SUCCESS"
#define SEVERITY_WARNING_STR "WANRING"
#define SEVERITY_ERROR_STR "ERROR"

using std::string;

namespace affix_base {
	namespace enums {
		enum severity {
			unknown,
			success,
			warning,
			error
		};

		string to_string(severity a_severity);
	}
}