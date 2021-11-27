#pragma once
#include "pch.h"

namespace affix_base {
	namespace exceptions {

#ifdef _DEBUG
#define CATCH_FRIENDLY_ASSERT(expression, message) \
		if (!(expression)) throw message;
#else
#define CATCH_FRIENDLY_ASSERT(expression, message)
#endif

	}
}
