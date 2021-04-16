#pragma once
#include "pch.h"

namespace affix_base {
	namespace threading {
		class mutex {
		public:
			mutex();

		public:
			void lock();
			void unlock();

		private:

		};
	}
}