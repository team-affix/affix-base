#include "affix_base.h"

using namespace CryptoPP;
void write_to_console(vector<byte> v) {
	for (int i = 0; i < v.size(); i++)
		std::cout << std::to_string((uint8_t)v[i]) << ", ";
}

using namespace affix_base::networking;
using namespace affix_base::cryptography;

int main() {

	using namespace affix_base::networking;
	using namespace affix_base::cryptography;

	using affix_base::callback::watch;

	bool m_continue = true;

	vector<watch> l_watches;

	std::thread watch_thread([&] {
		while(m_continue)
			for (int i = 0; i < l_watches.size(); i++)
				l_watches[i].process();
	});

	int int_to_watch = 0;

	l_watches.push_back(WATCH(int_to_watch == 10, [&] { std::cout << "eyyy" << std::endl; }));

	for (int i = 0; true; i++) {
		int_to_watch = i % 10000000;
	}

	std::cin.get();

	return EXIT_SUCCESS;

}