#pragma once
#include <string>
#include <vector>
#include <iostream>

#define dll __declspec(dllexport)

namespace affix_base {
	namespace crypto {

		class rsa;

		class rsa_public_key : public std::string { public: rsa_public_key() {} rsa_public_key(std::string a_base) : std::string(a_base) {} };
		class rsa_private_key : public std::string { public: rsa_private_key() {}  rsa_private_key(std::string a_base) : std::string(a_base) {} };

		struct rsa_keys {
		public:
			rsa_public_key public_key;
			rsa_private_key private_key;
		};

		class rsa {
		protected:
			void* rsa_csp = nullptr;

		public:
			rsa_keys keys;

		public:
			dll virtual ~rsa();
			dll rsa(size_t a_key_size);
			dll rsa(rsa_keys a_keys);
			dll rsa(rsa_public_key a_public_key);
			dll rsa(rsa_private_key a_private_key);
			dll std::vector<uint8_t> encrypt(std::vector<uint8_t>& a_vec);
			dll std::vector<uint8_t> decrypt(std::vector<uint8_t>& a_vec);

		};

	}
}

