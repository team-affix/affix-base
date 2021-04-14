#pragma once

namespace affix {
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