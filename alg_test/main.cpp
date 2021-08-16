#include "affix_base.h"

bool match_string_to_deque(const string& a_str, const std::deque<uint8_t>& a_deque) {
	for (int i = 0; i < a_str.size(); i++)
		if (a_str[i] != a_deque[i])
			return false;
	return true;
}

int main() {

	using namespace affix_base::data;

	vector<int> v1 = { 0, 1, 2, 3 };
	vector<double> v2 = cast<int, double>(v1);
	vector<double> v3 = range(v2, 0, 2);

	return EXIT_SUCCESS;
}