#pragma once

namespace affix_base {
	namespace memory {
		template<typename T>
		void wipe(T*& ptr) {
			if (ptr != nullptr) {
				delete ptr;
				ptr = nullptr;
			}
		}
	}
}
