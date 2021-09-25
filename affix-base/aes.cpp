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
