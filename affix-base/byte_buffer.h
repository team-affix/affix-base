#pragma once
#include "pch.h"
#include "ptr.h"
#include "builder.h"
#include <tuple>
#include "catch_friendly_assert.h"

namespace affix_base {
	namespace data {

		using std::deque;
		using std::vector;
		using affix_base::data::ptr;
		using affix_base::data::builder;
		using std::tuple;

		class byte_buffer {
		protected:
			deque<uint8_t> m_body;

		public:
			byte_buffer() {

			}
			byte_buffer(const vector<uint8_t>& a_body) {
				m_body.insert(m_body.begin(), a_body.begin(), a_body.end());
			}

		public:
			const size_t size() const {
				return m_body.size();
			}

		public:
			template<typename DATA_TYPE>
			bool push_back(const DATA_TYPE& a_data) {
				static_assert(std::is_standard_layout<DATA_TYPE>::value, "Data is not in a standard layout.");
				// PUSH DATA TO BACK OF STACK
				uint8_t* l_data_begin = (uint8_t*)&a_data;
				uint8_t* l_data_end = l_data_begin + sizeof(DATA_TYPE);
				m_body.insert(m_body.end(), l_data_begin, l_data_end);
				return true;
			}
			template<typename DATA_TYPE>
			bool push_front(const DATA_TYPE& a_data) {
				static_assert(std::is_standard_layout<DATA_TYPE>::value, "Data is not in a standard layout.");
				// PUSH DATA TO FRONT OF STACK
				uint8_t* l_data_begin = (uint8_t*)&a_data;
				uint8_t* l_data_end = l_data_begin + sizeof(DATA_TYPE);
				m_body.insert(m_body.begin(), l_data_begin, l_data_end);
				return true;
			}
			template<typename DATA_TYPE>
			bool pop_back(DATA_TYPE& a_data) {
				static_assert(std::is_standard_layout<DATA_TYPE>::value, "Data is not in a standard layout.");
				if (m_body.size() < sizeof(DATA_TYPE)) return false;

				// POP DATA FROM BACK OF STACK
				std::copy(m_body.end() - sizeof(DATA_TYPE), m_body.end(), (uint8_t*)&a_data);
				m_body.erase(m_body.end() - sizeof(DATA_TYPE), m_body.end());

				return true;
			}
			template<typename DATA_TYPE>
			bool pop_front(DATA_TYPE& a_data) {
				static_assert(std::is_standard_layout<DATA_TYPE>::value, "Data is not in a standard layout.");
				if (m_body.size() < sizeof(DATA_TYPE)) return false;

				// POP DATA FROM FRONT OF STACK
				std::copy(m_body.begin(), m_body.begin() + sizeof(DATA_TYPE), (uint8_t*)&a_data);
				m_body.erase(m_body.begin(), m_body.begin() + sizeof(DATA_TYPE));

				return true;
			}
			template<typename DATA_TYPE>
			bool push_back(const vector<DATA_TYPE>& a_vec) {
				
				// STORE SIZE OF VECTOR AT FRONT IN CASE OF POP_FRONT CALL
				if (!push_back(uint32_t(a_vec.size()))) return false;

				// STORE DATA IN FRONT-FACING ORDER ALWAYS
				for (int i = 0; i < a_vec.size(); i++)
					if (!push_back(a_vec[i])) return false;

				// STORE SIZE OF VECTOR AT END (AS WELL) IN CASE OF POP_BACK CALL
				if (!push_back(uint32_t(a_vec.size()))) return false;

				return true;
			}
			template<typename DATA_TYPE>
			bool push_front(const vector<DATA_TYPE>& a_vec) {

				// STORE SIZE OF VECTOR AT END IN CASE OF POP_FRONT CALL
				if (!push_front(uint32_t(a_vec.size()))) return false;

				// STORE DATA IN FRONT-FACING ORDER ALWAYS
				for (int i = a_vec.size() - 1; i >= 0; i--)
					if (!push_front(a_vec[i])) return false;

				// STORE SIZE OF VECTOR AT FRONT (AS WELL) IN CASE OF POP_BACK CALL
				if (!push_front(uint32_t(a_vec.size()))) return false;

				return true;
			}
			template<typename DATA_TYPE>
			bool pop_back(vector<DATA_TYPE>& a_vec) {

				// POP SIZE OF VECTOR FROM BACK
				uint32_t vec_size = 0;
				if (!pop_back(vec_size)) return false;
				a_vec.resize(vec_size);

				// POP ELEMENTS IN FRONT-FACING ORDER
				for (int i = vec_size - 1; i >= 0; i--) {
					if (!pop_back(a_vec[i])) return false;
				}

				// REMOVE THE POP_FRONT SIZE
				if (!pop_back(vec_size)) return false;

				return true;
			}
			template<typename DATA_TYPE>
			bool pop_front(vector<DATA_TYPE>& a_vec) {

				// POP SIZE OF VECTOR FROM FRONT
				uint32_t vec_size = 0;
				if (!pop_front(vec_size)) return false;
				a_vec.resize(vec_size);

				// POP ELEMENTS IN FRONT-FACING ORDER
				for (int i = 0; i < vec_size; i++) {
					if (!pop_front(a_vec[i])) return false;
				}

				// REMOVE THE POP_BACK SIZE
				if (!pop_front(vec_size)) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool push_back(const tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS PUSH TUPLE IN FRONT-FACING ORDER
				if (!push_back(std::get<0>(a_tuple))) return false;
				if (!push_back(std::get<1>(a_tuple))) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool push_front(const tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS PUSH TUPLE IN FRONT-FACING ORDER
				if (!push_back(std::get<1>(a_tuple))) return false;
				if (!push_back(std::get<0>(a_tuple))) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool pop_back(tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS POP TUPLE IN FRONT-FACING ORDER
				if (!pop_back(std::get<1>(a_tuple))) return false;
				if (!pop_back(std::get<0>(a_tuple))) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool pop_front(tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS POP TUPLE IN FRONT-FACING ORDER
				if (!pop_front(std::get<0>(a_tuple))) return false;
				if (!pop_front(std::get<1>(a_tuple))) return false;

				return true;
			}

		public:
			vector<uint8_t> data() const {
				vector<uint8_t> result;
				result.insert(result.end(), m_body.begin(), m_body.end());
				return result;
			}

		};

	}
}
