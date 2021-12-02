#pragma once
#include "pch.h"

namespace affix_base {
	namespace ios {

		void file_write(
			const std::string& a_path,
			const std::vector<uint8_t>& l_bytes
		);

		void file_read(
			const std::string& a_path,
			std::vector<uint8_t>& l_bytes
		);

	}
}
