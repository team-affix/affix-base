#pragma once
#include "pch.h"
#include "cryptopp/files.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/rsa.h"
#include "cryptopp/sha.h"
#include "cryptopp/pssr.h"
#include "vector_extensions.h"
#include <fstream>

namespace affix_base {
    namespace cryptography {

        /// <summary>
        /// Holds an RSA Private key and an RSA public key.
        /// </summary>
        struct rsa_key_pair {
            CryptoPP::RSA::PrivateKey private_key;
            CryptoPP::RSA::PublicKey public_key;
        };

        /// <summary>
        /// Generates an RSA key pair with the provided key size (in bytes).
        /// </summary>
        /// <param name="a_key_size"></param>
        /// <returns></returns>
        rsa_key_pair rsa_generate_key_pair(
            uint32_t a_key_size);
        
        /// <summary>
        /// Exports the base64 version of the RSA private key as a string.
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_ostream"></param>
        std::string rsa_to_base64_string(
            const CryptoPP::RSA::PrivateKey& a_private_key
        );

        /// <summary>
        /// Exports the base64 version of the RSA public key as a string.
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_ostream"></param>
        std::string rsa_to_base64_string(
            const CryptoPP::RSA::PublicKey& a_public_key
        );

        /// <summary>
        /// Imports the RSA private key from a base64 string.
        /// </summary>
        /// <returns></returns>
        bool rsa_from_base64_string(
            CryptoPP::RSA::PrivateKey& a_private_key,
            const std::string& a_base_64_string
        );

        /// <summary>
        /// Imports the RSA public key from a base64 string.
        /// </summary>
        /// <returns></returns>
        bool rsa_from_base64_string(
            CryptoPP::RSA::PublicKey& a_public_key,
            const std::string& a_base_64_string
        );

        /// <summary>
        /// Exports an RSA private key to an output file.
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_file_name"></param>
        void rsa_export(
            const CryptoPP::RSA::PrivateKey& a_private_key,
            const std::string& a_file_name);

        /// <summary>
        /// Exports an RSA public key to an output file.
        /// </summary>
        /// <param name="a_public_key"></param>
        /// <param name="a_file_name"></param>
        void rsa_export(
            const CryptoPP::RSA::PublicKey& a_public_key,
            const std::string& a_file_name);

        /// <summary>
        /// Exports an RSA private key to a vector of bytes.
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_output"></param>
        void rsa_export(
            const CryptoPP::RSA::PrivateKey& a_private_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Exports an RSA public key to a vector of bytes.
        /// </summary>
        /// <param name="a_public_key"></param>
        /// <param name="a_output"></param>
        void rsa_export(
            const CryptoPP::RSA::PublicKey& a_public_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Imports an RSA private key from a file.
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_file_name"></param>
        void rsa_import(
            CryptoPP::RSA::PrivateKey& a_private_key,
            const std::string& a_file_name);

        /// <summary>
        /// Imports an RSA public key from a file.
        /// </summary>
        /// <param name="a_public_key"></param>
        /// <param name="a_file_name"></param>
        void rsa_import(
            CryptoPP::RSA::PublicKey& a_public_key,
            const std::string& a_file_name);

        /// <summary>
        /// Imports an RSA private key from a vector of bytes.
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_input"></param>
        void rsa_import(
            CryptoPP::RSA::PrivateKey& a_private_key,
            const std::vector<CryptoPP::byte>& a_input);

        /// <summary>
        /// Imports an RSA public key from a vector of bytes.
        /// </summary>
        /// <param name="a_public_key"></param>
        /// <param name="a_input"></param>
        void rsa_import(
            CryptoPP::RSA::PublicKey& a_public_key,
            const std::vector<CryptoPP::byte>& a_input);

        /// <summary>
        /// Tries to import an RSA private key from a file. (success returns true)
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_file_name"></param>
        bool rsa_try_import(
            CryptoPP::RSA::PrivateKey& a_private_key,
            const std::string& a_file_name);

        /// <summary>
        /// Tries to import an RSA public key from a file. (success returns true)
        /// </summary>
        /// <param name="a_public_key"></param>
        /// <param name="a_file_name"></param>
        bool rsa_try_import(
            CryptoPP::RSA::PublicKey& a_public_key,
            const std::string& a_file_name);

        /// <summary>
        /// Tries to import an RSA private key from a vector of bytes. (success returns true)
        /// </summary>
        /// <param name="a_private_key"></param>
        /// <param name="a_input"></param>
        bool rsa_try_import(
            CryptoPP::RSA::PrivateKey& a_private_key,
            const std::vector<CryptoPP::byte>& a_input);

        /// <summary>
        /// Tries to import an RSA public key from a vector of bytes. (success returns true)
        /// </summary>
        /// <param name="a_public_key"></param>
        /// <param name="a_input"></param>
        bool rsa_try_import(
            CryptoPP::RSA::PublicKey& a_public_key,
            const std::vector<CryptoPP::byte>& a_input);

        /// <summary>
        /// Encrypts a vector of bytes with an RSA public key.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_public_key"></param>
        std::vector<CryptoPP::byte> rsa_encrypt(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PublicKey& a_public_key);

        /// <summary>
        /// Decrypts a vector of bytes using an RSA private key.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_private_key"></param>
        /// <returns></returns>
        std::vector<CryptoPP::byte> rsa_decrypt(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PrivateKey& a_private_key);

        /// <summary>
        /// Encrypts a vector in fixed size chunks using an RSA public key.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_public_key"></param>
        /// <returns></returns>
        std::vector<CryptoPP::byte> rsa_encrypt_in_chunks(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PublicKey& a_public_key);

        /// <summary>
        /// Decrypts a vector in fixed size chunks using an RSA private key.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_private_key"></param>
        /// <returns></returns>
        std::vector<CryptoPP::byte> rsa_decrypt_in_chunks(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PrivateKey& a_private_key);

        /// <summary>
        /// Encrypts a stream using an RSA public key.
        /// This transformation occurs in blocks or "chunks" to prevent memory overflow.
        /// </summary>
        /// <param name="a_input_file"></param>
        /// <param name="a_output_file"></param>
        /// <param name="a_public_key"></param>
        void rsa_encrypt(
            std::istream& a_input_stream,
            std::ostream& a_output_stream,
            const CryptoPP::RSA::PublicKey a_public_key
        );

        /// <summary>
        /// Decrypts a stream using an RSA private key.
        /// This transformation occurs in blocks or "chunks" to prevent memory overflow.
        /// </summary>
        /// <param name="a_input_file"></param>
        /// <param name="a_output_file"></param>
        /// <param name="a_private_key"></param>
        void rsa_decrypt(
            std::istream& a_input_stream,
            std::ostream& a_output_stream,
            const CryptoPP::RSA::PrivateKey a_private_key
        );

        /// <summary>
        /// Signs a message using an RSA private key.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_private_key"></param>
        /// <returns></returns>
        std::vector<CryptoPP::byte> rsa_sign(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PrivateKey& a_private_key);

        /// <summary>
        /// Verifies a message's authenticity using an RSA public key and signature.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_signature"></param>
        /// <param name="a_public_key"></param>
        /// <returns></returns>
        bool rsa_verify(
            const std::vector<CryptoPP::byte>& a_input,
            const std::vector<CryptoPP::byte>& a_signature,
            const CryptoPP::RSA::PublicKey& a_public_key);

        /// <summary>
        /// Tries to encrypt a vector of bytes using an RSA public key. (success returns true)
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_public_key"></param>
        /// <param name="a_output"></param>
        /// <returns></returns>
        bool rsa_try_encrypt(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PublicKey& a_public_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Tries to decrypt a vector of bytes using an RSA private key. (success returns true)
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_private_key"></param>
        /// <param name="a_output"></param>
        /// <returns></returns>
        bool rsa_try_decrypt(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PrivateKey& a_private_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Tries to encrypt a vector of bytes in fixed size chunks using an RSA public key. (success returns true)
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_public_key"></param>
        /// <param name="a_output"></param>
        /// <returns></returns>
        bool rsa_try_encrypt_in_chunks(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PublicKey& a_public_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Tries tp decrypt a vector of bytes in fixed size chunks using an RSA private key. (success returns true)
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_private_key"></param>
        /// <param name="a_output"></param>
        /// <returns></returns>
        bool rsa_try_decrypt_in_chunks(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PrivateKey& a_private_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Tries to sign a vector of bytes using an RSA private key. (success returns true)
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_private_key"></param>
        /// <param name="a_output"></param>
        /// <returns></returns>
        bool rsa_try_sign(
            const std::vector<CryptoPP::byte>& a_input,
            const CryptoPP::RSA::PrivateKey& a_private_key,
            std::vector<CryptoPP::byte>& a_output);

        /// <summary>
        /// Tries to verify a message's authenticity using an RSA public key and signature.
        /// Success in this case DOES NOT indicate that a message is in fact authentic. Instead it suggests that the verification procedure completed without errors. (success returns true)
        /// The authenticity of the message will be sent to the a_output boolean, where true = authentic.
        /// </summary>
        /// <param name="a_input"></param>
        /// <param name="a_signature"></param>
        /// <param name="a_public_key"></param>
        /// <param name="a_output"></param>
        /// <returns></returns>
        bool rsa_try_verify(
            const std::vector<CryptoPP::byte>& a_input,
            const std::vector<CryptoPP::byte>& a_signature,
            const CryptoPP::RSA::PublicKey& a_public_key,
            bool& a_output);

    }
}