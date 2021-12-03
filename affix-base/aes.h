#pragma once
#include "pch.h"
#include "vector_extensions.h"
#include "cryptopp/aes.h"
#include <istream>
#include <ostream>

namespace affix_base {
	namespace cryptography {

		/// <summary>
		/// Generates an AES key.
		/// </summary>
		/// <returns></returns>
		std::vector<CryptoPP::byte> aes_generate_key();

		/// <summary>
		/// Generates an AES initialization vector.
		/// </summary>
		/// <returns></returns>
		std::vector<CryptoPP::byte> aes_generate_iv();

		/// <summary>
		/// Generates a zeroed out AES initialization vector.
		/// </summary>
		/// <returns></returns>
		std::vector<CryptoPP::byte> aes_generate_iv_zeroed();

		/// <summary>
		/// Encrypts a vector of bytes using AES.
		/// </summary>
		/// <param name="a_data"></param>
		/// <param name="a_key"></param>
		/// <param name="a_iv"></param>
		/// <returns></returns>
		std::vector<CryptoPP::byte> aes_encrypt(
			const std::vector<CryptoPP::byte>& a_data,
			const std::vector<CryptoPP::byte>& a_key,
			std::vector<CryptoPP::byte> a_iv = {});

		/// <summary>
		/// Decrypts a vector of bytes using AES.
		/// </summary>
		/// <param name="a_data"></param>
		/// <param name="a_key"></param>
		/// <param name="a_iv"></param>
		/// <returns></returns>
		std::vector<CryptoPP::byte> aes_decrypt(
			const std::vector<CryptoPP::byte>& a_data,
			const std::vector<CryptoPP::byte>& a_key,
			std::vector<CryptoPP::byte> a_iv = {});

		/// <summary>
		/// Encrypts a stream using AES.
		/// This transformation occurs in blocks or "chunks" to prevent memory overflow.
		/// </summary>
		/// <param name="a_input_stream"></param>
		/// <param name="a_output_stream"></param>
		/// <param name="a_key"></param>
		/// <param name="a_iv"></param>
		void aes_encrypt(
			std::istream& a_input_stream,
			std::ostream& a_output_stream,
			const std::vector<CryptoPP::byte> a_key,
			std::vector<CryptoPP::byte> a_iv = {}
		);

		/// <summary>
		/// Decrypts a stream using AES.
		/// This transformation occurs in blocks or "chunks" to prevent memory overflow.
		/// </summary>
		/// <param name="a_input_stream"></param>
		/// <param name="a_output_stream"></param>
		/// <param name="a_key"></param>
		/// <param name="a_iv"></param>
		void aes_decrypt(
			std::istream& a_input_stream,
			std::ostream& a_output_stream,
			const std::vector<CryptoPP::byte> a_key,
			std::vector<CryptoPP::byte> a_iv = {}
		);

		/// <summary>
		/// Increments an AES initialization vector.
		/// </summary>
		/// <param name="a_iv"></param>
		void aes_increment_iv(
			std::vector<CryptoPP::byte>& a_iv);

		/// <summary>
		/// Sets an AES initialization vector to be equal to a CryptoPP::Integer.
		/// </summary>
		/// <param name="a_iv"></param>
		/// <param name="a_int"></param>
		void aes_set_iv(
			std::vector<CryptoPP::byte>& a_iv,
			const CryptoPP::Integer& a_int);

	}
}
