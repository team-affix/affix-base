#include "crypto_rsa.h"
#include "clr_conversion.h"

using namespace affix_base::crypto;
using namespace affix_base::conversion;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Cryptography;

std::string to_string(String^ str) {
	std::string result;
	for (int i = 0; i < str->Length; i++)
		result += str[i];
	return result;
}

String^ from_string(std::string& str) {
	String^ result;
	for (int i = 0; i < str.size(); i++)
		result += str[i];
	return result;
}

rsa::~rsa() {
	GCHandle^ rsa_crypto_service_provider_handle = handle(rsa_csp);
	rsa_crypto_service_provider_handle->Free();
}

rsa::rsa(size_t a_key_size) {
	auto r = gcnew RSACryptoServiceProvider(a_key_size);
	keys.public_key = to_string(r->ToXmlString(false));
	keys.private_key = to_string(r->ToXmlString(true));
	rsa_csp = unmanaged(r);
}

rsa::rsa(rsa_keys a_keys) {
	keys = a_keys;
	auto r = gcnew RSACryptoServiceProvider();
	r->FromXmlString(from_string(keys.public_key));
	r->FromXmlString(from_string(keys.private_key));
	rsa_csp = unmanaged(r);
}

rsa::rsa(rsa_public_key a_public_key) {
	keys.public_key = a_public_key;
	auto r = gcnew RSACryptoServiceProvider();
	r->FromXmlString(from_string(keys.public_key));
	rsa_csp = unmanaged(r);
}

rsa::rsa(rsa_private_key a_private_key) {
	keys.private_key = a_private_key;
	auto r = gcnew RSACryptoServiceProvider();
	r->FromXmlString(from_string(keys.private_key));
	rsa_csp = unmanaged(r);
}

std::vector<uint8_t> rsa::encrypt(std::vector<uint8_t>& vec) {
	std::vector<uint8_t> result;
	RSACryptoServiceProvider^ r = (RSACryptoServiceProvider^)managed(rsa_csp);
	array<unsigned char>^ arr = gcnew array<unsigned char>(vec.size());
	for (int i = 0; i < vec.size(); i++) arr[i] = vec[i];
	arr = r->Encrypt(arr, true);
	for (int i = 0; i < arr->Length; i++) result.push_back((uint8_t)arr[i]);
	return result;
}

std::vector<uint8_t> rsa::decrypt(std::vector<uint8_t>& vec) {
	std::vector<uint8_t> result;
	RSACryptoServiceProvider^ r = (RSACryptoServiceProvider^)managed(rsa_csp);
	array<unsigned char>^ arr = gcnew array<unsigned char>(vec.size());
	for (int i = 0; i < vec.size(); i++) arr[i] = vec[i];
	arr = r->Decrypt(arr, true);
	for (int i = 0; i < arr->Length; i++) result.push_back((uint8_t)arr[i]);
	return result;
}