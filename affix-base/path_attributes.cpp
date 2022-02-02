#include "pch.h"
#include "path_attributes.h"
#include <filesystem>

std::string affix_base::files::get_unoccupied_path(
	const std::string& a_path
)
{
	size_t l_extension_index = a_path.find_last_of(".");

	// PATH NAME INVOLVES EXTENSION
	if (l_extension_index != std::string::npos)
	{
		std::string l_path_without_extension = a_path.substr(0, l_extension_index);
		std::string l_extension = a_path.substr(l_extension_index, a_path.size() - l_extension_index);

		for (int i = 0; true; i++)
		{
			std::string l_result = l_path_without_extension + "-" + std::to_string(i) + l_extension;
			if (!std::filesystem::exists(l_result))
				return l_result;
		}
	}
	// PATH NAME DOES NOT INVOLVE EXTENSION
	else
	{
		for (int i = 0; true; i++)
		{
			std::string l_result = a_path + "-" + std::to_string(i);
			if (!std::filesystem::exists(l_result))
				return l_result;
		}
	}
}
