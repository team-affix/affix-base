#pragma once
#include "pch.h"
#include "ptr.h"
#include "builder.h"
#include <tuple>

namespace affix_base {
	namespace networking {

		using std::deque;
		using std::vector;
		using affix_base::data::ptr;
		using affix_base::data::builder;
		using std::tuple;

		class message {
		protected:
			deque<uint8_t> m_body;

		public:
			message() {

			}
			message(const vector<uint8_t>& a_body) {
				m_body.insert(m_body.begin(), a_body.begin(), a_body.end());
			}

		public:
			const size_t size() const {
				return m_body.size();
			}
			template<typename DATA_TYPE>
			friend message& operator << (message& a_msg, const vector<DATA_TYPE>& a_vec) {
				a_msg << uint32_t(a_vec.size());
				for (int i = 0; i < a_vec.size(); i++)
					a_msg << a_vec[i];
				return a_msg;
			}
			template<typename DATA_TYPE>
			friend message& operator >> (message& a_msg, vector<DATA_TYPE>& a_vec) {
				uint32_t vec_size = 0;
				a_msg >> vec_size;
				a_vec.resize(vec_size);
				for (int i = 0; i < vec_size; i++) {
					a_msg >> a_vec[i];
				}
				return a_msg;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			friend message& operator << (message& a_msg, const tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {
				a_msg << std::get<0>(a_tuple) << std::get<1>(a_tuple);
				return a_msg;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			friend message& operator >> (message& a_msg, tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {
				a_msg >> std::get<0>(a_tuple) >> std::get<1>(a_tuple);
				return a_msg;
			}
			template<typename DATA_TYPE>
			friend message& operator << (message& a_msg, const DATA_TYPE& a_data) {
				static_assert(std::is_standard_layout<DATA_TYPE>::value, "Data is not in a standard layout.");

				// PUSH DATA TO BACK OF STACK
				uint8_t* l_data_begin = (uint8_t*)&a_data;
				uint8_t* l_data_end = l_data_begin + sizeof(DATA_TYPE);
				a_msg.m_body.insert(a_msg.m_body.end(), l_data_begin, l_data_end);
				return a_msg;
			}
			template<typename DATA_TYPE>
			friend message& operator >> (message& a_msg, DATA_TYPE& a_data) {
				static_assert(std::is_standard_layout<DATA_TYPE>::value, "Data is not in a standard layout.");
				assert(a_msg.m_body.size() >= sizeof(DATA_TYPE));

				// POP DATA FROM FRONT OF STACK
				std::copy(a_msg.m_body.begin(), a_msg.m_body.begin() + sizeof(DATA_TYPE), (uint8_t*)&a_data);
				a_msg.m_body.erase(a_msg.m_body.begin(), a_msg.m_body.begin() + sizeof(DATA_TYPE));
				return a_msg;
			}

		public:
			deque<uint8_t>& body() {
				return m_body;
			}
			vector<uint8_t> serialize() const {
				vector<uint8_t> result;
				result.insert(result.end(), m_body.begin(), m_body.end());
				return result;
			}

		};

	}
}
