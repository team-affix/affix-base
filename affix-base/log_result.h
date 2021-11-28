#pragma once
#include "pch.h"
#include "severity.h"

namespace affix_base {
	namespace logging {

		void log_result(std::string a_message, affix_base::enums::severity a_severity = affix_base::enums::severity::unknown);

	}
}