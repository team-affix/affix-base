#include "pch.h"
#include "sha.h"

using namespace affix_base;
using namespace CryptoPP;
using std::vector;

void data::sha256_digest(const vector<uint8_t>& a_input, vector<uint8_t>& a_output, size_t a_truncated_digest_size) {
	assert(a_truncated_digest_size <= 32);
	SHA256 hash;
	hash.Update((const byte*)a_input.data(), a_input.size());
	a_output.resize(a_truncated_digest_size);
	hash.TruncatedFinal((byte*)a_output.data(), a_truncated_digest_size);
}

bool data::sha256_try_digest(const vector<uint8_t>& a_input, vector<uint8_t>& a_output, size_t a_truncated_digest_size) {
	try {
		sha256_digest(a_input, a_output, a_truncated_digest_size);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool data::sha256_validate(const vector<uint8_t>& a_input, const vector<uint8_t>& a_digest) {
	vector<uint8_t> l_digest;
	sha256_digest(a_input, l_digest, a_digest.size());
	return std::equal(l_digest.begin(), l_digest.end(), a_digest.begin(), a_digest.end());
}

bool data::sha256_try_validate(const vector<uint8_t>& a_input, const vector<uint8_t>& a_digest, bool& a_output) {
	try {
		a_output = sha256_validate(a_input, a_digest);
		return true;
	}
	catch (...) {
		return false;
	}
}

bool data::sha256_try_digest(
	std::istream& a_istream,
	std::vector<uint8_t>& a_output
)
{
	if (!a_istream.good())
		return false;

	SHA256 hash;

	std::vector<char> l_input_bytes(1000);

	for (uint64_t l_block = 0; a_istream.peek() != EOF; l_block++)
	{
		// Read file IN BLOCKS. DO NOT HASH AT EVERY BYTE.

		for (
			int i = 0;
			i < l_input_bytes.size() &&
			a_istream.peek() != EOF; i++
			)
		{
			a_istream.get(l_input_bytes[i]);
		}

		hash.Update((const byte*)l_input_bytes.data(), l_input_bytes.size());

	}

	a_output.resize(SHA256::DIGESTSIZE);
	hash.TruncatedFinal((byte*)a_output.data(), SHA256::DIGESTSIZE);

	return true;

}
