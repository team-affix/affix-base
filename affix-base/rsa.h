#pragma once
#include "pch.h"
#include "cryptopp/files.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/sha.h"
#include "cryptopp/pssr.h"
#include "vector_extensions.h"
#include <vector>

namespace affix_base {
    namespace cryptography {

        using namespace CryptoPP;
        using affix_base::data::range;
        using std::vector;

        struct rsa_key_pair {
            RSA::PrivateKey private_key;
            RSA::PublicKey public_key;
        };

        rsa_key_pair rsa_generate_key_pair(uint32_t a_key_size) {
            AutoSeededRandomPool random;
            RSA::PrivateKey priKey;
            priKey.GenerateRandomWithKeySize(random, a_key_size);
            RSA::PublicKey pubKey(priKey);
            return { priKey, pubKey };
        }

        void rsa_export(const RSA::PrivateKey& a_private_key, const string& a_file_name) {
            FileSink fs(a_file_name.c_str(), true);
            a_private_key.Save(fs);
        }

        void rsa_export(const RSA::PublicKey& a_public_key, const string& a_file_name) {
            FileSink fs(a_file_name.c_str(), true);
            a_public_key.Save(fs);
        }

        void rsa_export(const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            VectorSink vs(a_output);
            a_private_key.Save(vs);
        }

        void rsa_export(const RSA::PublicKey& a_public_key, vector<byte>& a_output) {
            VectorSink vs(a_output);
            a_public_key.Save(vs);
        }

        void rsa_import(RSA::PrivateKey& a_private_key, const string& a_file_name) {
            FileSource fs(a_file_name.c_str(), true);
            a_private_key.Load(fs);
        }

        void rsa_import(RSA::PublicKey& a_public_key, const string& a_file_name) {
            FileSource fs(a_file_name.c_str(), true);
            a_public_key.Load(fs);
        }

        void rsa_import(RSA::PrivateKey& a_private_key, const vector<byte>& a_input) {
            VectorSource vs(a_input, true);
            a_private_key.Load(vs);
        }

        void rsa_import(RSA::PublicKey& a_public_key, const vector<byte>& a_input) {
            VectorSource vs(a_input, true);
            a_public_key.Load(vs);
        }

        vector<byte> rsa_encrypt(const vector<byte>& a_input, const RSA::PublicKey& a_public_key) {
            AutoSeededRandomPool random;
            RSAES<OAEP<SHA256>>::Encryptor encryptor(a_public_key);
            SecByteBlock cipher(encryptor.CiphertextLength(a_input.size()));
            encryptor.Encrypt(random, a_input.data(), a_input.size(), cipher);
            vector<byte> result(cipher.size());
            memcpy(result.data(), cipher.data(), cipher.size());
            return result;
        }

        vector<byte> rsa_decrypt(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key) {
            AutoSeededRandomPool random;
            RSAES<OAEP<SHA256>>::Decryptor decryptor(a_private_key);
            SecByteBlock plain(decryptor.MaxPlaintextLength(a_input.size()));
            DecodingResult decoding_result = decryptor.Decrypt(random, a_input.data(), a_input.size(), plain);
            plain.resize(decoding_result.messageLength);
            vector<byte> result(plain.size());
            memcpy(result.data(), plain.data(), plain.size());
            return result;
        }

        vector<vector<byte>> rsa_encrypt_in_chunks(const vector<byte>& a_input, const RSA::PublicKey& a_public_key) {
            RSAES<OAEP<SHA256>>::Encryptor encryptor(a_public_key);
            vector<vector<byte>> result;
            for (int i = 0; i < a_input.size(); i += encryptor.FixedMaxPlaintextLength()) {
                size_t bytes_remaining = a_input.size() - i;
                vector<byte> chunk = range(a_input, i, std::min(encryptor.FixedMaxPlaintextLength(), bytes_remaining));
                result.push_back(rsa_encrypt(chunk, a_public_key));
            }
            return result;
        }

        vector<byte> rsa_decrypt_in_chunks(const vector<vector<byte>>& a_input, const RSA::PrivateKey& a_private_key) {
            vector<byte> result;
            for (int i = 0; i < a_input.size(); i++) {
                const vector<byte>& chunk = a_input[i];
                vector<byte> decrypted = rsa_decrypt(chunk, a_private_key);
                result.insert(result.end(), decrypted.begin(), decrypted.end());
            }
            return result;
        }

        vector<byte> rsa_sign(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key) {
            AutoSeededRandomPool random;
            RSASS<PSS, SHA256>::Signer signer(a_private_key);
            SecByteBlock signature(signer.MaxSignatureLength());
            size_t length = signer.SignMessage(random, a_input.data(), a_input.size(), signature);
            signature.resize(length);
            vector<byte> result(signature.size());
            memcpy(result.data(), signature.data(), signature.size());
            return result;
        }

        bool rsa_verify(const vector<byte>& a_input, const vector<byte>& a_signature, const RSA::PublicKey& a_public_key) {
            AutoSeededRandomPool random;
            RSASS<PSS, SHA256>::Verifier verifier(a_public_key);
            return verifier.VerifyMessage(a_input.data(), a_input.size(), a_signature.data(), a_signature.size());
        }

        bool rsa_try_encrypt(const vector<byte>& a_input, const RSA::PublicKey& a_public_key, vector<byte>& a_output) {
            try {
                a_output = rsa_encrypt(a_input, a_public_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool rsa_try_decrypt(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            try {
                a_output = rsa_decrypt(a_input, a_private_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool rsa_try_encrypt_in_chunks(const vector<byte>& a_input, const RSA::PublicKey& a_public_key, vector<vector<byte>>& a_output) {
            try {
                a_output = rsa_encrypt_in_chunks(a_input, a_public_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool rsa_try_decrypt_in_chunks(const vector<vector<byte>>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            try {
                a_output = rsa_decrypt_in_chunks(a_input, a_private_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool rsa_try_sign(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            try {
                a_output = rsa_sign(a_input, a_private_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool rsa_try_verify(const vector<byte>& a_input, const vector<byte>& a_signature, const RSA::PublicKey& a_public_key, bool& a_output) {
            try {
                a_output = rsa_verify(a_input, a_signature, a_public_key);
            }
            catch (...) {
                return false;
            }
        }

    }
}