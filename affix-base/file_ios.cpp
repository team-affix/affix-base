#include "pch.h"
#include "file_ios.h"
#include <fstream>

using namespace affix_base;

bool files::file_write(
	const std::string& a_path,
	const std::vector<uint8_t>& a_bytes
)
{
	std::ofstream l_ofs(a_path, std::ios::out | std::ios::binary);

    if (!l_ofs.is_open())
        return false;

	l_ofs.write((const char*)a_bytes.data(), a_bytes.size());
	l_ofs.close();

    return true;

}

bool files::file_read(
	const std::string& a_path,
	std::vector<uint8_t>& a_bytes
)
{
    std::ifstream file(a_path, std::ios::binary);

    if (!file.is_open())
        return false;

    file.unsetf(std::ios::skipws);

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();

    // RESET HEAD
    file.seekg(0, std::ios::beg);

    // RESIZE BYTE VECTOR
    a_bytes.reserve(fileSize);

    a_bytes.insert(a_bytes.begin(),
        std::istream_iterator<uint8_t>(file),
        std::istream_iterator<uint8_t>());

    file.close();

    return true;

}
