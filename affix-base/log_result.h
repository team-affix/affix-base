#pragma once
#include "severity.h"
#include <string>

using affix_base::enums::severity;
using std::string;

namespace affix_base {
	namespace logging {
		void log_result(string a_message, severity a_severity = severity::unknown);
	}
}