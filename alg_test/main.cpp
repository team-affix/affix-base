#include "affix_base.h"
#include <iostream>
#include <Windows.h>



void a_changed_callback(int a) {
	std::cout << a << std::endl;
}

void rand_changed_callback(int r) {

}

vector<function<void()>> tasks;

int main() {



	return 0;
}