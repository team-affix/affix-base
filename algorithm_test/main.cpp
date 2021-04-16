#include "watch.h"
#include <iostream>

using namespace affix_base::callback;

void on_variable_update(int& val) {

}

class my_class {
public:
	int variable = 0;
};

int main() {

	watch<int>::start_thread();



	my_class mc;
	watch<int> w(mc.variable, on_variable_update);
	w.start();

	return 0;
}