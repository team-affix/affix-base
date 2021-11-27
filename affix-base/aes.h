#pragma once
#include "pch.h"
#include "vector_extensions.h"
#include "cryptopp/aes.h"

namespace affix_base {
	namespace cryptography {

		using namespace CryptoPP;
		using std::vector;
		using std::string;
		using affix_base::data::range;

		vector<byte> aes_generate_key();
		vector<byte> aes_generate_iv();

		vector<byte> aes_generate_iv_zeroed();

		vector<byte> aes_encrypt(const vector<byte>& a_data, const vector<byte>& a_key, vector<byte> a_iv = {});
		vector<byte> aes_decrypt(const vector<byte>& a_data, const vector<byte>& a_key, vector<byte> a_iv = {});

		void aes_increment_iv(vector<byte>& a_iv);
		void aes_set_iv(vector<byte>& a_iv, const Integer& a_int);

	}
}
