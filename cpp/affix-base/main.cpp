#include "persistent_thread.h"
#include "persistent_thread_group.h"
#include <iostream>
#include "monitor.h"

using affix::threading::persistent_thread;
using affix::threading::persistent_thread_group;
using affix::callback::monitor;

int main() {

	persistent_thread_group ptg;;
	ptg.push_back(persistent_thread([&]() { std::cout << "1" << std::endl; }));
	ptg.push_back(persistent_thread([&]() { std::cout << "2" << std::endl; }));
	ptg.push_back(persistent_thread([&]() { std::cout << "3" << std::endl; }));
	ptg.push_back(persistent_thread([&]() { std::cout << "4" << std::endl; }));
	ptg.push_back(persistent_thread([&]() { std::cout << "5" << std::endl; }));
	ptg.push_back(persistent_thread([&]() { std::cout << "6" << std::endl; }));

	ptg.compile();
	ptg.execute();
	ptg.join();

	return 0;
}