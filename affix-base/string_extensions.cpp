#include "pch.h"
#include "string_extensions.h"

using namespace affix_base;
using std::vector;
using std::string;

vector<string> data::string_split(
	const string& a_string,
	const char& a_char
)
{
	vector<string> result = { "" };
	for (int i = 0; i < a_string.size(); i++)
		if (a_string[i] != a_char) {
			result.back() += a_string[i];
		}
		else {
			result.push_back("");
		}
	return result;
}

vector<string> data::string_split_respect_quotes(
	const string& a_string,
	const char& a_split_char
)
{
	vector<string> result = { "" };
	bool respect = false;
	for (int i = 0; i < a_string.size(); i++) {
		const char& l_char = a_string[i];
		if (l_char == '\"')
			respect = !respect;
		else if (l_char == a_split_char && !respect)
			result.push_back("");
		else
			result.back() += l_char;
	}
	return result;
}

string data::string_join(
	const vector<string>& a_strings
)
{
	string result;
	for (int i = 0; i < a_strings.size(); i++)
		result += a_strings[i];
	return result;
}

string data::string_trim(
	const string& a_string,
	const char& a_char
)
{
	return data::string_join(data::string_split(a_string, a_char));
}

string data::string_trim(
	const string& a_string,
	const vector<char>& a_chars
)
{
	string result = a_string;
	for (int i = 0; i < a_chars.size(); i++)
		result = data::string_trim(result, a_chars[i]);
	return result;
}

string data::string_trim_right_until(
	const string& a_string,
	const char& a_char
)
{
	string l_result = a_string;
	for (int i = l_result.size() - 1; i >= 0; i--) {
		if (l_result[i] != a_char)
			l_result.pop_back();
		else
			break;
	}
	return l_result;
}

string data::string_trim_left_until(
	const string& a_string,
	const char& a_char
)
{
	string l_result = a_string;
	for (int i = 0; i < l_result.size();) {
		if (l_result[i] != a_char)
			l_result.erase(l_result.begin());
		else
			break;
	}
	return l_result;
}

size_t data::string_find(
	const string& a_string,
	const char& a_char,
	const size_t& a_find_index
)
{
	int l_find_index = 0;
	for (int i = 0; i < a_string.size(); i++)
		if (a_string[i] == a_char)
			if (l_find_index == a_find_index)
				return i;
			else
				l_find_index++;
	return a_string.size();
}

std::vector<size_t> data::string_find_all(
	const std::string& a_data,
	const std::string& a_query
)
{
	std::vector<size_t> l_result;

	for (size_t l_offset = 0; true;)
	{
		size_t l_found_index = a_data.find(a_query, l_offset);
		if (l_found_index != string::npos)
		{
			l_result.push_back(l_found_index);
			l_offset = l_found_index + a_query.size();
		}
		else
		{
			return l_result;
		}
	}

}

bool data::string_can_query(
	const string& a_data,
	const string& a_query
)
{
	vector<string> match_split = data::string_split(a_query, '*');

	size_t next_match_start = 0;
	for (int i = 0; i < match_split.size() - 1; i++) {

		const string& l_string = match_split[i];
		if (l_string != "") {
			size_t this_match = a_data.find(l_string, next_match_start);
			if (this_match == string::npos)
				return false;
			if (i == 0 && this_match != 0)
				return false;
			next_match_start = this_match + l_string.size();
		}

	}

	std::string final_search = match_split.back();

	if (final_search != "")
	{
		if (a_data.size() < final_search.size())
			return false;

		std::string data_end = a_data.substr(a_data.size() - final_search.size(), final_search.size());

		if (final_search != data_end)
			return false;
	}

	return true;

}

bool data::string_any_can_query(
	const std::string& a_data,
	const std::vector<std::string>& a_queries
)
{
	return std::find_if(a_queries.begin(), a_queries.end(),
		[&a_data](const string& a_elem) 
		{
			return data::string_can_query(a_data, a_elem);
		}) != a_queries.end();
}
