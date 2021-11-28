#pragma once
#include "pch.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"
#include <string>

namespace affix_base {
	namespace data {

		void sha256_digest(const std::vector<uint8_t>& a_input, std::vector<uint8_t>& a_output, size_t a_truncated_digest_size = 32);
		bool sha256_try_digest(const std::vector<uint8_t>& a_input, std::vector<uint8_t>& a_output, size_t a_truncated_digest_size = 32);

		bool sha256_validate(const std::vector<uint8_t>& a_input, const std::vector<uint8_t>& a_digest);
		bool sha256_try_validate(const std::vector<uint8_t>& a_input, const std::vector<uint8_t>& a_digest, bool& a_output);

	}
}
