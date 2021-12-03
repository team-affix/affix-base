#include "pch.h"
#include "aes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rijndael.h"
#include "cryptopp/cryptlib.h"
#include "cryptopp/modes.h"
#include "cryptopp/files.h"

using namespace affix_base;
using CryptoPP::AutoSeededRandomPool;
using namespace CryptoPP;
using std::vector;
using std::string;
using affix_base::data::range;

const size_t AES_MAX_PLAINTEXT_LENGTH_FOR_SIZE_16_CIPHER = 15;

vector<byte> cryptography::aes_generate_key() {
	AutoSeededRandomPool l_random;
	SecByteBlock l_key(AES::DEFAULT_KEYLENGTH);
	l_random.GenerateBlock(l_key.data(), l_key.size());
	vector<byte> result;
	result.insert(result.end(), l_key.data(), l_key.data() + l_key.size());
	return result;
}

vector<byte> cryptography::aes_generate_iv() {
	AutoSeededRandomPool l_random;
	SecByteBlock l_iv(AES::BLOCKSIZE);
	l_random.GenerateBlock(l_iv.data(), l_iv.size());
	vector<byte> result;
	result.insert(result.end(), l_iv.begin(), l_iv.end());
	return result;
}

vector<byte> cryptography::aes_generate_iv_zeroed() {
	return vector<byte>(AES::BLOCKSIZE);
}

vector<byte> cryptography::aes_encrypt(const vector<byte>& a_data, const vector<byte>& a_key, vector<byte> a_iv) {
	
	// ENSURE VALID INITIALIZATION VECTOR
	if (a_iv.size() != AES::BLOCKSIZE)
		a_iv = vector<byte>(AES::BLOCKSIZE);

	// CREATE ENCRYPTOR
	CBC_Mode<AES>::Encryption l_encryptor;
	l_encryptor.SetKeyWithIV(a_key.data(), a_key.size(), a_iv.data());
	std::string l_encrypted;
	
	// PERFORM ACTUAL ENCRYPTION
	VectorSource l_source(a_data, true,
		new StreamTransformationFilter(l_encryptor,
			new StringSink(l_encrypted)
		)
	);

	// HANDLE RESULT
	vector<byte> result;
	result.insert(result.end(), l_encrypted.begin(), l_encrypted.end());

	return result;

}

vector<byte> cryptography::aes_decrypt(const vector<byte>& a_data, const vector<byte>& a_key, vector<byte> a_iv) {

	// ENSURE VALID INITIALIZATION VECTOR
	if (a_iv.size() != AES::BLOCKSIZE)
		a_iv = vector<byte>(AES::BLOCKSIZE);

	// CREATE DECRYPTOR
	CBC_Mode<AES>::Decryption l_decryptor;
	l_decryptor.SetKeyWithIV(a_key.data(), a_key.size(), a_iv.data());
	std::string l_decrypted;

	// PERFORM ACTUAL DECRYPTION
	VectorSource l_source(a_data, true,
		new StreamTransformationFilter(l_decryptor,
			new StringSink(l_decrypted)
		)
	);

	// HANDLE RESULT
	vector<byte> result;
	result.insert(result.end(), l_decrypted.begin(), l_decrypted.end());

	return result;

}

void cryptography::aes_encrypt(
	std::istream& a_input_stream,
	std::ostream& a_output_stream,
	const std::vector<CryptoPP::byte> a_key,
	std::vector<CryptoPP::byte> a_iv
)
{
	// ENSURE VALID INITIALIZATION VECTOR
	if (a_iv.size() != AES::BLOCKSIZE)
		a_iv = vector<byte>(AES::BLOCKSIZE);

	// CREATE ENCRYPTOR
	CBC_Mode<AES>::Encryption l_encryptor;
	l_encryptor.SetKeyWithIV(a_key.data(), a_key.size(), a_iv.data());
	
	std::vector<CryptoPP::byte> l_input_buffer(AES::BLOCKSIZE);
	std::vector<CryptoPP::byte> l_output_buffer;

	for (size_t block = 0; a_input_stream.peek() != EOF; block++)
	{
		size_t l_bytes_to_process = 0;

		for (;
			l_bytes_to_process < AES_MAX_PLAINTEXT_LENGTH_FOR_SIZE_16_CIPHER &&
			a_input_stream.peek() != EOF;
			l_bytes_to_process++)
		{
			// READ IN 1 BYTE FROM INPUT
			a_input_stream.read((char*)l_input_buffer.data() + l_bytes_to_process, 1);
		}
		
		// PREPARE OUTPUT BUFFER
		l_output_buffer.clear();

		// ENCRYPT INPUT BUFFER
		ArraySource l_source(l_input_buffer.data(), l_bytes_to_process, true,
			new StreamTransformationFilter(l_encryptor,
				new VectorSink(l_output_buffer)
			)
		);

		// WRITE ENCRYPTED DATA
		a_output_stream.write((const char*)l_output_buffer.data(), l_output_buffer.size());

	}
}

void cryptography::aes_decrypt(
	std::istream& a_input_stream,
	std::ostream& a_output_stream,
	const std::vector<CryptoPP::byte> a_key,
	std::vector<CryptoPP::byte> a_iv
)
{
	// ENSURE VALID INITIALIZATION VECTOR
	if (a_iv.size() != AES::BLOCKSIZE)
		a_iv = vector<byte>(AES::BLOCKSIZE);

	// CREATE DECRYPTOR
	CBC_Mode<AES>::Decryption l_decryptor;
	l_decryptor.SetKeyWithIV(a_key.data(), a_key.size(), a_iv.data());

	std::vector<CryptoPP::byte> l_input_buffer(AES::BLOCKSIZE);
	std::vector<CryptoPP::byte> l_output_buffer;

	for (size_t block = 0; a_input_stream.peek() != EOF; block++)
	{
		size_t l_bytes_to_process = 0;

		for (;
			l_bytes_to_process < AES::BLOCKSIZE &&
			a_input_stream.peek() != EOF;
			l_bytes_to_process++)
		{
			// READ IN 1 BYTE FROM INPUT
			a_input_stream.read((char*)l_input_buffer.data() + l_bytes_to_process, 1);
		}

		// PREPARE OUTPUT BUFFER
		l_output_buffer.clear();

		// DECRYPT INPUT BUFFER
		ArraySource l_source(l_input_buffer.data(), l_bytes_to_process, true, 
			new StreamTransformationFilter(l_decryptor,
				new VectorSink(l_output_buffer))
		);

		// WRITE DECRYPTED DATA
		a_output_stream.write((const char*)l_output_buffer.data(), l_output_buffer.size());

	}
}

void cryptography::aes_increment_iv(vector<byte>& a_iv) {
	a_iv.resize(AES::BLOCKSIZE);
	Integer l_int;
	l_int.Decode(a_iv.data(), a_iv.size());
	l_int++;
	l_int.Encode(a_iv.data(), a_iv.size());
}

void cryptography::aes_set_iv(vector<byte>& a_iv, const Integer& a_int) {
	a_iv.resize(AES::BLOCKSIZE);
	a_int.Encode(a_iv.data(), a_iv.size());
}
