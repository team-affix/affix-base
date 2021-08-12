#include "affix_base.h"

bool match_string_to_deque(const string& a_str, const std::deque<uint8_t>& a_deque) {
	for (int i = 0; i < a_str.size(); i++)
		if (a_str[i] != a_deque[i])
			return false;
	return true;
}

int main() {

	using affix_base::data::builder;
	using affix_base::data::ts_deque;

	ts_deque<string> d;

	string str = "testing123";

	d.push_back(str);

	string str2 = str;

	return EXIT_SUCCESS;
}