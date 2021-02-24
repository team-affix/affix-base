#include "persistent_thread.h"
#include <iostream>
#include "monitor.h"

using affix::threading::persistent_thread;
using affix::callback::monitor;

int main() {

	persistent_thread thd([&]() { std::cout << "hello" << std::endl; });

	thd.execute();
	std::cout << "test" << std::endl;
	thd.join();

	

	return 0;
}