#include "pch.h"
#include "severity.h"

using namespace affix_base;
using std::string;

string enums::to_string(severity a_severity) {
	switch (a_severity) {
	case severity::unknown:
		return SEVERITY_UNKNOWN_STR;
	case severity::success:
		return SEVERITY_SUCCESS_STR;
	case severity::warning:
		return SEVERITY_WARNING_STR;
	case severity::error:
		return SEVERITY_ERROR_STR;
	}
}