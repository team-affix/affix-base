#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {

		class builder {
		protected:
			size_t m_capacity = 0;
			size_t m_buffer_used_size = 0;
			std::vector<uint8_t> m_buffer;

		public:
			builder();
			builder(size_t a_capacity);

		public:
			// Returns true if the full capacity is now occupied.
			bool ready() const;
			// Returns true if the full capacity is NOW occupied.
			friend bool operator << (builder& a_builder, const uint8_t& a_uint) {
				a_builder.m_buffer[a_builder.m_buffer_used_size] = a_uint;
				a_builder.m_buffer_used_size++;
				return a_builder.ready();
			}

		public:
			size_t capacity();
			uint8_t* data();

		public:
			std::vector<uint8_t>& buffer();
			template<typename T>
			T build() {
				T result;
				memcpy(&result, data(), sizeof(T));
				return result;
			}

		};
	}
}