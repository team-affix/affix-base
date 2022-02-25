#include "pch.h"
#include "rsa.h"
#include "cryptopp/files.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/sha.h"
#include "cryptopp/pssr.h"
#include "vector_extensions.h"
#include "byte_buffer.h"
#include <fstream>
#include <filesystem>
#include "cryptopp/base64.h"

using namespace CryptoPP;
using affix_base::data::range;
using std::vector;
using std::string;
using affix_base::data::byte_buffer;
namespace fs = std::filesystem;
using namespace affix_base;

namespace affix_base {
    namespace cryptography {

        rsa_key_pair cryptography::rsa_generate_key_pair(uint32_t a_key_size) {
            AutoSeededRandomPool random;
            RSA::PrivateKey priKey;
            priKey.GenerateRandomWithKeySize(random, a_key_size);
            RSA::PublicKey pubKey(priKey);
            return { priKey, pubKey };
        }

        bool cryptography::rsa_to_base64_string(
            const CryptoPP::RSA::PrivateKey& a_private_key,
            std::string& a_string
        )
        {
            try
            {
                a_string.clear();
                CryptoPP::Base64Encoder l_string_sink(new StringSink(a_string), false);
                a_private_key.DEREncode(l_string_sink);
                l_string_sink.MessageEnd();
                return true;
            }
            catch (std::exception a_exception)
            {
                return false;
            }
        }

        bool cryptography::rsa_to_base64_string(
            const CryptoPP::RSA::PublicKey& a_public_key,
            std::string& a_string
        )
        {
            try
            {
                a_string.clear();
                CryptoPP::Base64Encoder l_string_sink(new StringSink(a_string), false);
                a_public_key.DEREncode(l_string_sink);
                l_string_sink.MessageEnd();
                return true;
            }
            catch (std::exception a_exception)
            {
                return false;
            }
        }

        bool cryptography::rsa_from_base64_string(
            CryptoPP::RSA::PrivateKey& a_private_key,
            const std::string& a_base_64_string
        )
        {
            try
            {
                StringSource ss(a_base_64_string, true, new Base64Decoder);
                a_private_key.BERDecode(ss);
                
                // Validate key integrity
                AutoSeededRandomPool l_random;
                return a_private_key.Validate(l_random, 3);
            }
            catch (...)
            {
                return false;
            }
        }

        bool cryptography::rsa_from_base64_string(
            CryptoPP::RSA::PublicKey& a_public_key,
            const std::string& a_base_64_string
        )
        {
            try
            {
                StringSource ss(a_base_64_string, true, new Base64Decoder);
                a_public_key.BERDecode(ss);

                // Validate key integrity
                AutoSeededRandomPool l_random;
                return a_public_key.Validate(l_random, 3);
            }
            catch (...)
            {
                return false;
            }
        }

        void cryptography::rsa_export(const RSA::PrivateKey& a_private_key, const string& a_file_name) {
            FileSink fs(a_file_name.c_str(), true);
            a_private_key.Save(fs);
        }

        void cryptography::rsa_export(const RSA::PublicKey& a_public_key, const string& a_file_name) {
            FileSink fs(a_file_name.c_str(), true);
            a_public_key.Save(fs);
        }

        void cryptography::rsa_export(const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            VectorSink vs(a_output);
            a_private_key.Save(vs);
        }

        void cryptography::rsa_export(const RSA::PublicKey& a_public_key, vector<byte>& a_output) {
            VectorSink vs(a_output);
            a_public_key.Save(vs);
        }

        void cryptography::rsa_import(RSA::PrivateKey& a_private_key, const string& a_file_name) {
            FileSource fs(a_file_name.c_str(), true);
            a_private_key.Load(fs);
        }

        void cryptography::rsa_import(RSA::PublicKey& a_public_key, const string& a_file_name) {
            FileSource fs(a_file_name.c_str(), true);
            a_public_key.Load(fs);
        }

        void cryptography::rsa_import(RSA::PrivateKey& a_private_key, const vector<byte>& a_input) {
            VectorSource vs(a_input, true);
            a_private_key.Load(vs);
        }

        void cryptography::rsa_import(RSA::PublicKey& a_public_key, const vector<byte>& a_input) {
            VectorSource vs(a_input, true);
            a_public_key.Load(vs);
        }

        bool cryptography::rsa_try_import(RSA::PrivateKey& a_private_key, const string& a_file_name) {
            try {
                rsa_import(a_private_key, a_file_name);
                AutoSeededRandomPool random;
                return a_private_key.Validate(random, 3);
            }
            catch (...) {
                return false;
            }
        }
        
        bool cryptography::rsa_try_import(RSA::PublicKey& a_public_key, const string& a_file_name) {
            try {
                rsa_import(a_public_key, a_file_name);
                AutoSeededRandomPool random;
                return a_public_key.Validate(random, 3);
            }
            catch (...) {
                return false;
            }
        }
        
        bool cryptography::rsa_try_import(RSA::PrivateKey& a_private_key, const vector<byte>& a_input) {
            try {
                rsa_import(a_private_key, a_input);
                AutoSeededRandomPool random;
                return a_private_key.Validate(random, 3);
            }
            catch (...) {
                return false;
            }
        }
        
        bool cryptography::rsa_try_import(RSA::PublicKey& a_public_key, const vector<byte>& a_input) {
            try {
                rsa_import(a_public_key, a_input);
                AutoSeededRandomPool random;
                return a_public_key.Validate(random, 3);
            }
            catch (...) {
                return false;
            }
        }

        vector<byte> cryptography::rsa_encrypt(const vector<byte>& a_input, const RSA::PublicKey& a_public_key) {
            AutoSeededRandomPool random;
            RSAES<OAEP<SHA256>>::Encryptor encryptor(a_public_key);
            SecByteBlock cipher(encryptor.CiphertextLength(a_input.size()));
            encryptor.Encrypt(random, a_input.data(), a_input.size(), cipher);
            vector<byte> result(cipher.size());
            memcpy(result.data(), cipher.data(), cipher.size());
            return result;
        }

        vector<byte> cryptography::rsa_decrypt(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key) {
            AutoSeededRandomPool random;
            RSAES<OAEP<SHA256>>::Decryptor decryptor(a_private_key);
            SecByteBlock plain(decryptor.MaxPlaintextLength(a_input.size()));
            DecodingResult decoding_result = decryptor.Decrypt(random, a_input.data(), a_input.size(), plain);
            plain.resize(decoding_result.messageLength);
            vector<byte> result(plain.size());
            memcpy(result.data(), plain.data(), plain.size());
            return result;
        }

        vector<byte> cryptography::rsa_encrypt_in_chunks(const vector<byte>& a_input, const RSA::PublicKey& a_public_key) {
            RSAES<OAEP<SHA256>>::Encryptor encryptor(a_public_key);
            vector<vector<byte>> result;
            for (int i = 0; i < a_input.size(); i += encryptor.FixedMaxPlaintextLength()) {
                size_t bytes_remaining = a_input.size() - i;
                vector<byte> chunk = range(a_input, i, std::min(encryptor.FixedMaxPlaintextLength(), bytes_remaining));
                result.push_back(rsa_encrypt(chunk, a_public_key));
            }
            byte_buffer l_buffer;
            l_buffer.push_back(result);
            return l_buffer.data();
        }

        vector<byte> cryptography::rsa_decrypt_in_chunks(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key) {
            byte_buffer l_buffer(a_input);
            vector<vector<byte>> l_input;
            l_buffer.pop_front(l_input);
            vector<byte> result;
            for (int i = 0; i < l_input.size(); i++) {
                const vector<byte>& chunk = l_input[i];
                vector<byte> decrypted = rsa_decrypt(chunk, a_private_key);
                result.insert(result.end(), decrypted.begin(), decrypted.end());
            }
            return result;
        }

        void cryptography::rsa_encrypt(
            std::istream& a_input_stream,
            std::ostream& a_output_stream,
            const CryptoPP::RSA::PublicKey a_public_key
        )
        {
            AutoSeededRandomPool l_random;
            RSAES<OAEP<SHA256>>::Encryptor l_encryptor(a_public_key);

            std::vector<byte> l_input_buffer(l_encryptor.FixedMaxPlaintextLength());
            std::vector<byte> l_output_buffer(l_encryptor.FixedCiphertextLength());

            for (size_t block = 0; a_input_stream.peek() != EOF; block++)
            {
                size_t l_bytes_to_process = 0;

                for (;
                    l_bytes_to_process < l_encryptor.FixedMaxPlaintextLength() &&
                    a_input_stream.peek() != EOF;
                    l_bytes_to_process++)
                {
                    // LOAD 1 BYTE INTO BUFFER
                    a_input_stream.read((char*)l_input_buffer.data() + l_bytes_to_process, 1);
                }

                // ENCRYPT BUFFER
                l_encryptor.Encrypt(l_random, l_input_buffer.data(), l_bytes_to_process, l_output_buffer.data());

                // WRITE OUTPUT
                a_output_stream.write((const char*)l_output_buffer.data(), l_encryptor.CiphertextLength(l_bytes_to_process));

            }

        }

        void rsa_decrypt(
            std::istream& a_input_stream,
            std::ostream& a_output_stream,
            const CryptoPP::RSA::PrivateKey a_private_key
        )
        {
            AutoSeededRandomPool l_random;
            RSAES<OAEP<SHA256>>::Decryptor l_decryptor(a_private_key);

            std::vector<byte> l_input_buffer(l_decryptor.FixedCiphertextLength());
            std::vector<byte> l_output_buffer(l_decryptor.FixedMaxPlaintextLength());

            for (size_t block = 0; a_input_stream.peek() != EOF; block++)
            {
                size_t l_bytes_to_process = 0;
                for (;
                    l_bytes_to_process < l_decryptor.FixedCiphertextLength() &&
                    a_input_stream.peek() != EOF;
                    l_bytes_to_process++)
                {
                    // LOAD 1 BYTE INTO BUFFER
                    a_input_stream.read((char*)l_input_buffer.data() + l_bytes_to_process, 1);
                }

                // DECRYPT BUFFER
                DecodingResult l_decoding_result = l_decryptor.Decrypt(l_random, l_input_buffer.data(), l_bytes_to_process, l_output_buffer.data());

                // WRITE OUTPUT
                a_output_stream.write((const char*)l_output_buffer.data(), l_decoding_result.messageLength);

            }

        }

        vector<byte> cryptography::rsa_sign(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key) {
            AutoSeededRandomPool random;
            RSASS<PSS, SHA256>::Signer signer(a_private_key);
            SecByteBlock signature(signer.MaxSignatureLength());
            size_t length = signer.SignMessage(random, a_input.data(), a_input.size(), signature);
            signature.resize(length);
            vector<byte> result(signature.size());
            memcpy(result.data(), signature.data(), signature.size());
            return result;
        }

        bool cryptography::rsa_verify(const vector<byte>& a_input, const vector<byte>& a_signature, const RSA::PublicKey& a_public_key) {
            AutoSeededRandomPool random;
            RSASS<PSS, SHA256>::Verifier verifier(a_public_key);
            return verifier.VerifyMessage(a_input.data(), a_input.size(), a_signature.data(), a_signature.size());
        }

        bool cryptography::rsa_try_encrypt(const vector<byte>& a_input, const RSA::PublicKey& a_public_key, vector<byte>& a_output) {
            try {
                a_output = rsa_encrypt(a_input, a_public_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool cryptography::rsa_try_decrypt(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            try {
                a_output = rsa_decrypt(a_input, a_private_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool cryptography::rsa_try_encrypt_in_chunks(const vector<byte>& a_input, const RSA::PublicKey& a_public_key, vector<byte>& a_output) {
            try {
                a_output = rsa_encrypt_in_chunks(a_input, a_public_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool cryptography::rsa_try_decrypt_in_chunks(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            try {
                a_output = rsa_decrypt_in_chunks(a_input, a_private_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool cryptography::rsa_try_sign(const vector<byte>& a_input, const RSA::PrivateKey& a_private_key, vector<byte>& a_output) {
            try {
                a_output = rsa_sign(a_input, a_private_key);
                return a_output.size() > 0;
            }
            catch (...) {
                return false;
            }
        }

        bool cryptography::rsa_try_verify(const vector<byte>& a_input, const vector<byte>& a_signature, const RSA::PublicKey& a_public_key, bool& a_output) {
            try {
                a_output = rsa_verify(a_input, a_signature, a_public_key);
                return true;
            }
            catch (...) {
                return false;
            }
        }


    }
}

