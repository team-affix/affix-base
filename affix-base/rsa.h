#pragma once
#include "pch.h"
#include "cryptopp/files.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/sha.h"
#include "cryptopp/pssr.h"
#include "vector_extensions.h"

namespace affix_base {
    namespace cryptography {

        using namespace CryptoPP;
        using affix_base::data::range;
        using std::vector;
        using std::string;

        struct rsa_key_pair {
            RSA::PrivateKey private_key;
            RSA::PublicKey public_key;
        };

        rsa_key_pair rsa_generate_key_pair(uint32_t a_key_size);

        void rsa_export(const RSA::PrivateKey& a_private_key, const string& a_file_name);
        void rsa_export(const RSA::PublicKey& a_public_key, const string& a_file_name);
        void rsa_export(const RSA::PrivateKey& a_private_key, vector<byte>& a_output);
        void rsa_export(const RSA::PublicKey& a_public_key, vector<byte>& a_output);

        void rsa_import(RSA::PrivateKey& a_private_key, const string& a_file_name);
        void rsa_import(RSA::PublicKey& a_public_key, const string& a_file_name);
        void rsa_import(RSA::PrivateKey& a_private_key, const vector<byte>& a_input);
        void rsa_import(RSA::PublicKey& a_public_key, const vector<byte>& a_input);

        bool rsa_try_import(RSA::PrivateKey& a_private_key, const string& a_file_name);
        bool rsa_try_import(RSA::PublicKey& a_public_key, const string& a_file_name);
        bool rsa_try_import(RSA::PrivateKey& a_private_key, const vector<byte>& a_input);
        bool rsa_try_import(RSA::PublicKey& a_public_key, const vector<byte>& a_input);

        vector<byte> rsa_encrypt(const vector<byte>& a_input, const RSA::PublicKey& a_public_key);
        vector<byte> rsa_decrypt(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key);

        vector<vector<byte>> rsa_encrypt_in_chunks(const vector<byte>& a_input, const RSA::PublicKey& a_public_key);
        vector<byte> rsa_decrypt_in_chunks(const vector<vector<byte>>& a_input, const RSA::PrivateKey& a_private_key);

        vector<byte> rsa_sign(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key);
        bool rsa_verify(const vector<byte>& a_input, const vector<byte>& a_signature, const RSA::PublicKey& a_public_key);

        bool rsa_try_encrypt(const vector<byte>& a_input, const RSA::PublicKey& a_public_key, vector<byte>& a_output);
        bool rsa_try_decrypt(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output);

        bool rsa_try_encrypt_in_chunks(const vector<byte>& a_input, const RSA::PublicKey& a_public_key, vector<vector<byte>>& a_output);
        bool rsa_try_decrypt_in_chunks(const vector<vector<byte>>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output);

        bool rsa_try_sign(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output);
        bool rsa_try_verify(const vector<byte>& a_input, const vector<byte>& a_signature, const RSA::PublicKey& a_public_key, bool& a_output);

    }
}