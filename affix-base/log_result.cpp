#include "log_result.h"
#include <iostream>

using namespace affix_base;
using namespace affix_base::enums;

void logging::log_result(string a_message, severity a_severity) {
	std::cout << "LOG [" << to_string(a_severity) << "] : " << a_message << std::endl;
}