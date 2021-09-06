#pragma once
#include "pch.h"

namespace affix_base {
	namespace exceptions {

#define CATCH_FRIENDLY_ASSERT(expression, message) \
		if (!(expression)) throw message;

	}
}
