#include "node.h"
#include <iostream>

using namespace affix::data;

int main() {

	node<int> n = 10;
	n.insert(1, 11);

	std::cout << n.fwd(1).val() << std::endl;

	return 0;
}