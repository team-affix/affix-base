#pragma once
#include "pch.h"
#include "ptr.h"
#include "builder.h"
#include <tuple>
#include "catch_friendly_assert.h"
#include <string>
#include "tree.h"

namespace affix_base {
	namespace data {

		class byte_buffer {
		protected:
			std::deque<uint8_t> m_body;

		public:
			byte_buffer() {

			}
			byte_buffer(const std::vector<uint8_t>& a_body) {
				m_body.insert(m_body.begin(), a_body.begin(), a_body.end());
			}

		public:
			const size_t size() const {
				return m_body.size();
			}

		public:
			template<typename DATA_TYPE>
			bool push_back(const DATA_TYPE& a_data) {
				if constexpr (std::is_standard_layout<DATA_TYPE>::value)
				{
					// PUSH DATA TO BACK OF STACK
					uint8_t* l_data_begin = (uint8_t*)&a_data;
					uint8_t* l_data_end = l_data_begin + sizeof(DATA_TYPE);
					m_body.insert(m_body.end(), l_data_begin, l_data_end);
					return true;
				}
				else
				{
					byte_buffer l_byte_buffer;
					if (!a_data.serialize(l_byte_buffer))
						return false;
					return push_back(l_byte_buffer);
				}
			}
			template<typename DATA_TYPE>
			bool push_front(const DATA_TYPE& a_data) {
				if constexpr (std::is_standard_layout<DATA_TYPE>::value)
				{
					// PUSH DATA TO FRONT OF STACK
					uint8_t* l_data_begin = (uint8_t*)&a_data;
					uint8_t* l_data_end = l_data_begin + sizeof(DATA_TYPE);
					m_body.insert(m_body.begin(), l_data_begin, l_data_end);
					return true;
				}
				else
				{
					byte_buffer l_byte_buffer;
					if (!a_data.serialize(l_byte_buffer))
						return false;
					return push_front(l_byte_buffer);
				}
			}
			template<typename DATA_TYPE>
			bool pop_back(DATA_TYPE& a_data) {
				if constexpr (std::is_standard_layout<DATA_TYPE>::value)
				{
					if (m_body.size() < sizeof(DATA_TYPE)) return false;

					// POP DATA FROM BACK OF STACK
					std::copy(m_body.end() - sizeof(DATA_TYPE), m_body.end(), (uint8_t*)&a_data);
					m_body.erase(m_body.end() - sizeof(DATA_TYPE), m_body.end());

					return true;
				}
				else
				{
					byte_buffer l_byte_buffer;
					if (!pop_back(l_byte_buffer))
						return false;
					return a_data.deserialize(l_byte_buffer);
				}
			}
			template<typename DATA_TYPE>
			bool pop_front(DATA_TYPE& a_data) {
				if constexpr (std::is_standard_layout<DATA_TYPE>::value)
				{
					if (m_body.size() < sizeof(DATA_TYPE)) return false;

					// POP DATA FROM FRONT OF STACK
					std::copy(m_body.begin(), m_body.begin() + sizeof(DATA_TYPE), (uint8_t*)&a_data);
					m_body.erase(m_body.begin(), m_body.begin() + sizeof(DATA_TYPE));

					return true;
				}
				else
				{
					byte_buffer l_byte_buffer;
					if (!pop_front(l_byte_buffer))
						return false;
					return a_data.deserialize(l_byte_buffer);
				}
			}
			template<>
			bool push_back<std::string>(const std::string& a_data)
			{
				// Push the size onto the stack twice, once before
				if (!push_back(a_data.size())) return false;

				for (int i = 0; i < a_data.size(); i++)
					if (!push_back(a_data[i])) return false;

				// and once after
				if (!push_back(a_data.size())) return false;

				return true;
			}
			template<>
			bool push_front<std::string>(const std::string& a_data)
			{
				// Push the size onto the stack twice, once before
				if (!push_front(a_data.size())) return false;

				for (int i = a_data.size() - 1; i >= 0; i--)
					if (!push_front(a_data[i])) return false;

				// and once after
				if (!push_front(a_data.size())) return false;

				return true;
			}
			template<>
			bool pop_back<std::string>(std::string& a_data)
			{
				size_t l_size;
				
				if (!pop_back(l_size)) return false;
				
				a_data.resize(l_size);

				for (int i = l_size - 1; i >= 0; i--)
					if (!pop_back(a_data[i])) return false;

				if (!pop_back(l_size)) return false;

				return true;

			}
			template<>
			bool pop_front<std::string>(std::string& a_data)
			{
				size_t l_size;

				if (!pop_front(l_size)) return false;

				a_data.resize(l_size);

				for (int i = 0; i < l_size; i++)
					if (!pop_front(a_data[i])) return false;

				if (!pop_front(l_size)) return false;

				return true;

			}
			template<typename DATA_TYPE>
			bool push_back(const std::vector<DATA_TYPE>& a_vec) {
				
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
			bool push_front(const std::vector<DATA_TYPE>& a_vec) {

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
			bool pop_back(std::vector<DATA_TYPE>& a_vec) {

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
			bool pop_front(std::vector<DATA_TYPE>& a_vec) {

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
			bool push_back(const std::tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS PUSH TUPLE IN FRONT-FACING ORDER
				if (!push_back(std::get<0>(a_tuple))) return false;
				if (!push_back(std::get<1>(a_tuple))) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool push_front(const std::tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS PUSH TUPLE IN FRONT-FACING ORDER
				if (!push_back(std::get<1>(a_tuple))) return false;
				if (!push_back(std::get<0>(a_tuple))) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool pop_back(std::tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS POP TUPLE IN FRONT-FACING ORDER
				if (!pop_back(std::get<1>(a_tuple))) return false;
				if (!pop_back(std::get<0>(a_tuple))) return false;

				return true;
			}
			template<typename DATA_TYPE_1, typename DATA_TYPE_2>
			bool pop_front(std::tuple<DATA_TYPE_1, DATA_TYPE_2>& a_tuple) {

				// ALWAYS POP TUPLE IN FRONT-FACING ORDER
				if (!pop_front(std::get<0>(a_tuple))) return false;
				if (!pop_front(std::get<1>(a_tuple))) return false;

				return true;
			}

			bool push_back(const byte_buffer& a_byte_buffer)
			{
				return push_back(a_byte_buffer.data());
			}
			bool push_front(const byte_buffer& a_byte_buffer)
			{
				return push_front(a_byte_buffer.data());
			}
			bool pop_back(byte_buffer& a_byte_buffer)
			{
				std::vector<uint8_t> l_byte_buffer_data;
				if (!pop_back(l_byte_buffer_data))
					return false;
				a_byte_buffer = byte_buffer(l_byte_buffer_data);
				return true;
			}
			bool pop_front(byte_buffer& a_byte_buffer)
			{
				std::vector<uint8_t> l_byte_buffer_data;
				if (!pop_front(l_byte_buffer_data))
					return false;
				a_byte_buffer = byte_buffer(l_byte_buffer_data);
				return true;
			}

			template<typename DATA_TYPE>
			bool push_back(
				const affix_base::data::tree<DATA_TYPE>& a_tree
			)
			{
				if (!push_back(a_tree.m_resource))
					return false;

				const std::vector<affix_base::data::tree<DATA_TYPE>>& l_subtrees(a_tree);

				if (!push_back(l_subtrees))
					return false;

				return true;

			}

			template<typename DATA_TYPE>
			bool push_front(
				const affix_base::data::tree<DATA_TYPE>& a_tree
			)
			{
				const std::vector<affix_base::data::tree<DATA_TYPE>>& l_subtrees(a_tree);

				if (!push_front(l_subtrees))
					return false;

				if (!push_front(a_tree.m_resource))
					return false;

				return true;

			}

			template<typename DATA_TYPE>
			bool pop_back(
				affix_base::data::tree<DATA_TYPE>& a_tree
			)
			{

				std::vector<affix_base::data::tree<DATA_TYPE>>& l_subtrees(a_tree);

				if (!pop_back(l_subtrees))
					return false;

				if (!pop_back(a_tree.m_resource))
					return false;

				return true;

			}

			template<typename DATA_TYPE>
			bool pop_front(
				affix_base::data::tree<DATA_TYPE>& a_tree
			)
			{
				if (!pop_front(a_tree.m_resource))
					return false;

				std::vector<affix_base::data::tree<DATA_TYPE>>& l_subtrees(a_tree);

				if (!pop_front(l_subtrees))
					return false;

				return true;

			}

		public:
			std::vector<uint8_t> data() const {
				std::vector<uint8_t> result;
				result.insert(result.end(), m_body.begin(), m_body.end());
				return result;
			}

		};

	}
}
