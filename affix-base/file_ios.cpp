#include "pch.h"
#include "file_ios.h"
#include <fstream>
#include "sha.h"

using namespace affix_base;

bool files::file_write(
	const std::string& a_path,
	const std::vector<uint8_t>& a_bytes
)
{
	std::ofstream l_ofs(a_path, std::ios::out | std::ios::binary | std::ios::trunc);

    if (!l_ofs.is_open() || !l_ofs.good())
    {
        l_ofs.close();
        return false;
    }

	l_ofs.write((const char*)a_bytes.data(), a_bytes.size());

    l_ofs.flush();

	l_ofs.close();

    return true;

}

bool files::file_read(
	const std::string& a_path,
	std::vector<uint8_t>& a_bytes
)
{
    a_bytes.clear();

    std::ifstream l_ifs(a_path, std::ios::binary | std::ios::in);

    

    if (!l_ifs.is_open() || !l_ifs.good())
    {
        l_ifs.close();
        return false;
    }

    char l_char = 0;

    while (l_ifs >> l_char)
        a_bytes.push_back((uint8_t)l_char);

    l_ifs.close();

    return true;

}

bool files::file_hash(
    const std::string& a_path,
    std::vector<uint8_t>& a_output
)
{
    using namespace CryptoPP;

    std::ifstream l_ifs(a_path, std::ios::in | std::ios::binary | std::ios::beg);

    return affix_base::data::sha256_try_digest(l_ifs, a_output);

}
