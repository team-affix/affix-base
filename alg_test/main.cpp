#include "affix_base.h"

bool match_string_to_deque(const string& a_str, const std::deque<uint8_t>& a_deque) {
	for (int i = 0; i < a_str.size(); i++)
		if (a_str[i] != a_deque[i])
			return false;
	return true;
}

int main() {

	using affix_base::data::rolling_buffer;

	string str_message = "AFFIX MESSAGE";

	rolling_buffer<uint8_t> byte_buffer(str_message.size());

	for (int i = 0; !match_string_to_deque(str_message, byte_buffer.buffer()); i++) {
		byte_buffer << str_message[i];
	}

	return EXIT_SUCCESS;
}