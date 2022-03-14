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

template<
	typename RETURN_TYPE = void,
	typename FUNCTION_IDENTIFIER_TYPE = std::string,
	typename CALL_IDENTIFIER_TYPE = std::string,
	typename ... PARAMETER_TYPES>
std::function<std::future<RETURN_TYPE>(PARAMETER_TYPES ...)> remote_function(
	const FUNCTION_IDENTIFIER_TYPE& a_function_identifier,
	const std::function<CALL_IDENTIFIER_TYPE(FUNCTION_IDENTIFIER_TYPE, affix_base::data::byte_buffer)>& a_initiate_remote_function_call,
	const std::function<affix_base::data::byte_buffer(CALL_IDENTIFIER_TYPE)>& a_get_result
)
{
	return std::function<std::future<RETURN_TYPE>(PARAMETER_TYPES ...)>(
		[&, a_function_identifier, a_initiate_remote_function_call, a_get_result](PARAMETER_TYPES ... a_params)
		{
			// Serialize all input parameters
			affix_base::data::byte_buffer l_input;

			if constexpr (sizeof...(PARAMETER_TYPES) > 0)
			{
				// Only try to serialize the parameters if there are parameters expected
				affix_base::data::serializable l_serializable(a_params...);
				l_serializable.serialize(l_input);
			}

			// Initiate the remote function call
			CALL_IDENTIFIER_TYPE l_call_identifier = a_initiate_remote_function_call(a_function_identifier, l_input);

			return std::async(std::launch::deferred,
				[&, a_get_result, l_call_identifier]
				{
					// Get the returned value from calling the remote function
					affix_base::data::byte_buffer l_result_byte_buffer = a_get_result(l_call_identifier);

					if constexpr (std::is_same<RETURN_TYPE, void>::value)
					{
						// Return type is void
						return;
					}
					else
					{
						// Return type is non-void
						RETURN_TYPE l_result;
						l_result_byte_buffer.pop_front(l_result);
						return l_result;
					}
				});
			
		});
}

int v(int a, int b)
{
	return a * b;
}

struct function_data_transfer_declaration
{
	std::string m_function_identifier;
	std::string m_call_identifier;
	affix_base::data::byte_buffer m_byte_buffer;
};

affix_base::threading::guarded_resource<std::vector<function_data_transfer_declaration>, affix_base::threading::cross_thread_mutex> i_function_data_transfers_to_remote;
affix_base::threading::guarded_resource<std::vector<function_data_transfer_declaration>, affix_base::threading::cross_thread_mutex> i_function_data_transfers_to_local;

void remote_main()
{
	using namespace affix_base::threading;

	std::map<std::string, std::function<affix_base::data::byte_buffer(affix_base::data::byte_buffer)>> l_local_function_map;

	l_local_function_map.insert({ 
		"multiply_doubles",
		local_function(std::function([&](double a, double b)
		{
			return a * b;
		})) 
	});


	while (true)
	{
		locked_resource l_function_data_transfers_to_remote = i_function_data_transfers_to_remote.lock();

		for (int i = l_function_data_transfers_to_remote->size() - 1; i >= 0; i--)
		{
			auto l_it = l_function_data_transfers_to_remote->begin() + i;

			// Extract data from iterator
			function_data_transfer_declaration l_result_data_transfer = *l_it;

			// Erase element from vector
			l_function_data_transfers_to_remote->erase(l_it);

			l_result_data_transfer.m_byte_buffer = l_local_function_map[l_result_data_transfer.m_function_identifier](l_result_data_transfer.m_byte_buffer);

			locked_resource l_function_data_transfers_to_local = i_function_data_transfers_to_local.lock();
			
			l_function_data_transfers_to_local->push_back(l_result_data_transfer);

		}

	}
}

int main() {

	using namespace affix_base::callback;
	using namespace affix_base::threading;
	using namespace affix_base::data;
	namespace fs = std::filesystem;
	
	auto multiply_doubles = remote_function<double, std::string, std::string, double, double>(std::string("multiply_doubles"),
		std::function([&](std::string a_function_identifier, affix_base::data::byte_buffer a_byte_buffer)
		{
			std::string l_call_identifier;
			l_call_identifier.resize(25);
			CryptoPP::AutoSeededRandomPool l_random;
			l_random.GenerateBlock((CryptoPP::byte*)l_call_identifier.data(), l_call_identifier.size());
			
			locked_resource l_function_data_transfers_to_remote = i_function_data_transfers_to_remote.lock();

			function_data_transfer_declaration l_function_data_transfer = {
				a_function_identifier,
				l_call_identifier,
				a_byte_buffer
			};

			l_function_data_transfers_to_remote->push_back(l_function_data_transfer);

			return l_call_identifier;

		}),
		std::function([&](std::string a_call_identifier)
		{
			while (true)
			{
				locked_resource l_function_data_transfers_to_local = i_function_data_transfers_to_local.lock();

				auto l_result = std::find_if(l_function_data_transfers_to_local->begin(), l_function_data_transfers_to_local->end(),
					[&](function_data_transfer_declaration& a_function_data_transfer_declaration)
					{
						return a_function_data_transfer_declaration.m_call_identifier == a_call_identifier;
					});

				if (l_result != l_function_data_transfers_to_local->end())
					return l_result->m_byte_buffer;

			}
		}));

	std::thread l_remote_thd(remote_main);

	// Wait for thread to start
	while (!l_remote_thd.joinable());

	std::future<double> l_future_result = multiply_doubles(2, 3);

	double l_result = l_future_result.get();

	l_remote_thd.join();

 	return EXIT_SUCCESS;

}
