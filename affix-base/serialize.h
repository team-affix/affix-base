#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {

		template<typename T>
		std::vector<uint8_t> serialize(const T& a_data) {
			std::vector<uint8_t> result(sizeof(T));
			memcpy(result.data(), &a_data, sizeof(T));
			return result;
		}

		template<typename T>
		T deserialize(const std::vector<uint8_t>& a_data) {
			assert(a_data.size() == sizeof(T));
			T result;
			memcpy(&result, a_data.data(), sizeof(T));
			return result;
		}

		template<typename T>
		bool try_serialize(const T& a_data, std::vector<uint8_t>& a_output) {
			try {
				a_output = serialize<T>(a_data);
				return true;
			}
			catch (...) {
				return false;
			}
		}

		template<typename T>
		bool try_deserialize(const std::vector<uint8_t>& a_data, T& a_output) {
			try {
				a_output = deserialize<T>(a_data);
				return true;
			}
			catch (...) {
				return false;
			}
		}

	}
}
