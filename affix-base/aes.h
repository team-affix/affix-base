#pragma once
#include "pch.h"
#include "vector_extensions.h"
#include "cryptopp/aes.h"

namespace affix_base {
	namespace cryptography {

		std::vector<CryptoPP::byte> aes_generate_key();

		std::vector<CryptoPP::byte> aes_generate_iv();

		std::vector<CryptoPP::byte> aes_generate_iv_zeroed();

		std::vector<CryptoPP::byte> aes_encrypt(
			const std::vector<CryptoPP::byte>& a_data,
			const std::vector<CryptoPP::byte>& a_key,
			std::vector<CryptoPP::byte> a_iv = {});

		std::vector<CryptoPP::byte> aes_decrypt(
			const std::vector<CryptoPP::byte>& a_data,
			const std::vector<CryptoPP::byte>& a_key,
			std::vector<CryptoPP::byte> a_iv = {});

		void aes_increment_iv(
			std::vector<CryptoPP::byte>& a_iv);

		void aes_set_iv(
			std::vector<CryptoPP::byte>& a_iv,
			const CryptoPP::Integer& a_int);

	}
}
