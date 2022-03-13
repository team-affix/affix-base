#include "affix_base.h"
#include <filesystem>
#include "serializable.h"
#include <tuple>
#include "cryptopp/osrng.h"
#include "guarded_resource.h"

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
std::function<affix_base::data::byte_buffer(affix_base::data::byte_buffer)> local_function(
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

template<typename T>
class async_result
{
protected:
	affix_base::threading::guarded_resource<bool, affix_base::threading::cross_thread_mutex> m_resource_available;
	T m_resource;

public:
	void set(
		const T& a_resource
	)
	{
		affix_base::threading::locked_resource l_resource_available = m_resource_available.lock();
		m_resource = a_resource;
		*l_resource_available = true;
	}

	T& get(

	)
	{
		while (true)
		{
			affix_base::threading::locked_resource l_resource_available = m_resource_available.lock();
			if (*l_resource_available)
			{
				break;
			}
		}
		return m_resource;
	}

};

template<typename RETURN_TYPE = void, typename ... PARAMETER_TYPES>
std::function<affix_base::data::ptr<async_result<RETURN_TYPE>>(PARAMETER_TYPES ...)> remote_function(
	const std::string& a_function_identifier,
	const std::function<void(affix_base::data::byte_buffer, affix_base::data::ptr<async_result<RETURN_TYPE>>)> a_call_remote_function
)
{
	return std::function<affix_base::data::ptr<async_result<RETURN_TYPE>>(PARAMETER_TYPES ...)>(
		[&, a_function_identifier, a_](PARAMETER_TYPES ... a_params)
		{
			affix_base::data::byte_buffer l_input;
			affix_base::data::serializable l_serializable(a_params);
			l_serializable.serialize(l_input);



		});
}

int v(int a, int b)
{
	return a * b;
}

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;

	/*std::map<std::string, std::function<byte_buffer(byte_buffer)>> l_local_function_map;
	l_local_function_map.insert({ "multiply_doubles", local_function(std::function([&](double a_a, double a_b) { return a_a * a_b; })) });*/

	std::shared_future<int> l_future;

	l_future.get();

 	return EXIT_SUCCESS;

}
