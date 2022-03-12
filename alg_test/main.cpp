#include "affix_base.h"
#include <filesystem>
#include "serializable.h"
#include <tuple>

using namespace affix_base::networking;
using namespace affix_base::cryptography;
using std::mutex;
using asio::ip::tcp;
using asio::ip::udp;
using asio::io_service;
using asio::io_context;
using affix_base::data::ptr;
using std::lock_guard;
using std::string;
namespace ip = asio::ip;
using asio::error_code;

template<typename RETURN_TYPE = void, typename ... PARAMETER_TYPES>
std::function<affix_base::data::byte_buffer(affix_base::data::byte_buffer)> external_function(
	const std::function<RETURN_TYPE(PARAMETER_TYPES ...)>& a_function
)
{
	return std::function<affix_base::data::byte_buffer(affix_base::data::byte_buffer)>(
		[&, a_function](affix_base::data::byte_buffer a_byte_buffer)
		{
			// Construct the resulting byte buffer
			affix_base::data::byte_buffer l_result;

			if constexpr (sizeof...(PARAMETER_TYPES) != 0)
			{
				// Construct a temporary place to store all of the deserialized fields
				std::tuple<PARAMETER_TYPES...> l_tuple;

				// Construct a serializer
				affix_base::data::ptr<affix_base::data::serializable> l_serializer;

				// Link up the serializer to the tuple's elements
				std::apply(
					[&](PARAMETER_TYPES&... a_params)
					{
						l_serializer = new affix_base::data::serializable(a_params...);
					}, l_tuple);

				// Deserialize the byte buffer
				l_serializer->deserialize(a_byte_buffer);

				if constexpr (std::is_same<RETURN_TYPE, void>::value)
				{
					std::apply(
						[&](PARAMETER_TYPES&... a_params)
						{
							a_function(a_params...);
						}, l_tuple);
				}
				else
				{
					std::apply(
						[&](PARAMETER_TYPES&... a_params)
						{
							RETURN_TYPE l_returned_value = a_function(a_params...);
							l_result.push_back(l_returned_value);
						}, l_tuple);
				}

			}
			else
			{
				if constexpr (std::is_same<RETURN_TYPE, void>::value)
				{
					a_function();
				}
				else
				{
					RETURN_TYPE l_returned_value = a_function();
					l_result.push_back(l_returned_value);
				}
			}

			return l_result;

		});
}

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;

	auto ef = external_function(
			std::function([&]()
			{
				
			}));

	byte_buffer l_input;
	std::string s = "yo";
	l_input.push_back(s);

	byte_buffer l_output = ef(l_input);
	std::string s2;
	l_output.pop_front(s2);

 	return EXIT_SUCCESS;

}
