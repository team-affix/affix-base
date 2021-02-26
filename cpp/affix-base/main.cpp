#include "persistent_thread.h"
#include "persistent_thread_group.h"
#include <iostream>
#include "monitor.h"

using affix::threading::persistent_thread;
using affix::threading::persistent_thread_group;
using affix::callback::monitor;

int main() {

	persistent_thread_group ptg({
		persistent_thread([&] {std::cout << "1 ts processed" << std::endl; }),
		persistent_thread([&] {std::cout << "1 ts processed" << std::endl; }),
		persistent_thread([&] {std::cout << "1 ts processed" << std::endl; }),
		persistent_thread([&] {std::cout << "1 ts processed" << std::endl; }),
		persistent_thread([&] {std::cout << "1 ts processed" << std::endl; }),
		persistent_thread([&] {std::cout << "1 ts processed" << std::endl; })
	});

	ptg.compile();
	ptg.execute();
	ptg.join();

	return 0;
}