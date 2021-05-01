#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace affix_base {
	namespace conversion {
		void* unmanaged(System::Object^ a_obj);
		System::Object^ managed(void* a_ptr);
		GCHandle^ handle(void* a_ptr);
	}
}