#include "affix_base.h"

//bool match_string_to_deque(const string& a_str, const std::deque<uint8_t>& a_deque) {
//	for (int i = 0; i < a_str.size(); i++)
//		if (a_str[i] != a_deque[i])
//			return false;
//	return true;
//}

int main() {

	using namespace affix_base::data;
	using namespace affix_base::cryptography;
	using namespace affix_base::timing;

	rsa_key_pair kp = rsa_generate_key_pair(1024);

	vector<byte> input = { 0, 1, 2, 3, 4 };
	vector<byte> encrypted = rsa_encrypt(input, kp.public_key);

	vector<byte> l_private_key;
	rsa_export(kp.private_key, l_private_key);

	rsa_key_pair kp2;

	rsa_import(kp2.private_key, l_private_key);

	vector<byte> decrypted = rsa_decrypt(encrypted, kp2.private_key);

	return EXIT_SUCCESS;

}