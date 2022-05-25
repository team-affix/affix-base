#pragma once
#include "pch.h"
#include "byte_buffer.h"

namespace affix_base {
	namespace files {

		void file_write(
			const std::string& a_path,
			const std::vector<uint8_t>& l_bytes
		);

		void file_read(
			const std::string& a_path,
			std::vector<uint8_t>& l_bytes
		);

		template<typename DATA_TYPE>
		bool file_write(
			const std::string& a_path,
			const DATA_TYPE& a_data
		)
		{
			affix_base::data::byte_buffer l_byte_buffer;

			if (!l_byte_buffer.push_back(a_data))
				return false;

			try
			{
				file_write(a_path, l_byte_buffer.data());
			}
			catch (std::exception)
			{
				return false;
			}

			return true;

		}

		template<typename DATA_TYPE>
		bool file_read(
			const std::string& a_path,
			DATA_TYPE& a_data
		)
		{
			std::vector<uint8_t> l_bytes;

			try
			{
				file_read(a_path, l_bytes);
			}
			catch (std::exception)
			{
				return false;
			}

			affix_base::data::byte_buffer l_byte_buffer(l_bytes);

			return l_byte_buffer.pop_front(a_data);

		}

	}
}
