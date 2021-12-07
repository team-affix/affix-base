#pragma once
#include "pch.h"

namespace affix_base {
	namespace data {

		/// <summary>
		/// Splits the string over all occurances of a character.
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_char"></param>
		/// <returns></returns>
		std::vector<std::string> string_split(
			const std::string& a_string, 
			const char& a_char);

		/// <summary>
		/// Splits a string over all occurances of a character, only if it is found outside a pair of quotation marks (...""...).
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_split_char"></param>
		/// <returns></returns>
		std::vector<std::string> string_split_respect_quotes(
			const std::string& a_string, 
			const char& a_split_char);

		/// <summary>
		/// Joins multiple strings together.
		/// </summary>
		/// <param name="a_strings"></param>
		/// <returns></returns>
		std::string string_join(
			const std::vector<std::string>& a_strings);

		/// <summary>
		/// Trims a string of all occurances of a character.
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_char"></param>
		/// <returns></returns>
		std::string string_trim(
			const std::string& a_string, 
			const char& a_char);

		/// <summary>
		/// Trims a string of all occurances a set of characters.
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_chars"></param>
		/// <returns></returns>
		std::string string_trim(
			const std::string& a_string, 
			const std::vector<char>& a_chars);

		/// <summary>
		/// Trims the right-hand side of a string until a specified character is found, at which point the function returns, not removing the specified character.
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_char"></param>
		std::string string_trim_right_until(
			const std::string& a_string, 
			const char& a_char);

		/// <summary>
		/// Trims the left-hand side of a string until a specified character is found, at which point the function returns, not removing the specified character.
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_char"></param>
		std::string string_trim_left_until(
			const std::string& a_string,
			const char& a_char);

		/// <summary>
		/// Identifies the location of a character within the string, with a specified index occurance of the character which we should find.
		/// If the character cannot be found within the string, the returned location will be equal to the size of the inputted string.
		/// </summary>
		/// <param name="a_string"></param>
		/// <param name="a_char"></param>
		/// <param name="a_find_index"></param>
		/// <returns></returns>
		size_t string_find(
			const std::string& a_string, 
			const char& a_char, 
			const size_t& a_find_index = 0
		);

		/// <summary>
		/// Finds all locations of a query string within another string. 
		/// </summary>
		/// <param name="a_data"></param>
		/// <param name="a_query"></param>
		/// <returns></returns>
		std::vector<size_t> string_find_all(
			const std::string& a_data,
			const std::string& a_query
		);

		/// <summary>
		/// Determines if a string (a_query) matches another string (a_data) while respecting wildcards (*) in the query. 
		/// </summary>
		/// <param name="a_data"></param>
		/// <param name="a_query"></param>
		/// <returns></returns>
		bool string_can_query(
			const std::string& a_data, 
			const std::string& a_query);

		/// <summary>
		/// Determines if any element of a list of strings (a_queries) matches another string (a_data) while respecting wildcards (*) in the query.
		/// </summary>
		/// <param name="a_data"></param>
		/// <param name="a_queries"></param>
		/// <returns></returns>
		bool string_any_can_query(
			const std::string& a_data, 
			const std::vector<std::string>& a_queries);

	}
}
