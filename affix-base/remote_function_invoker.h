#pragma once
#include "pch.h"
#include "byte_buffer.h"
#include "ptr.h"

namespace affix_base
{
	namespace distributed_computing
	{
		template<typename FUNCTION_IDENTIFIER_TYPE>
		class remote_function_invoker
		{
		public:
			template<typename RETURN_TYPE, typename ... SERIALIZED_PARAMETER_TYPES>
			std::pair<affix_base::data::byte_buffer, std::shared_future<RETURN_TYPE>> invoke(
				const std::launch& a_launch_method,
				const std::function<affix_base::data::byte_buffer()>& a_get_return,
				const FUNCTION_IDENTIFIER_TYPE& a_function_identifier,
				SERIALIZED_PARAMETER_TYPES... a_serialized_args
			)
			{
				affix_base::data::byte_buffer l_serialized_function_call_byte_buffer;

				// Push function identifier to byte buffer.
				l_serialized_function_call_byte_buffer.push_back(a_function_identifier);

				// Push function args to byte buffer.
				if constexpr (sizeof...(SERIALIZED_PARAMETER_TYPES) > 0)
				{
					l_serialized_function_call_byte_buffer.push_back(a_serialized_args...);
				}

				// Return a tuple of items
				return std::pair(
					// First item in tuple
					l_serialized_function_call_byte_buffer,
					//Second item in tuple is std::future
					std::async(a_launch_method,
					[&, a_get_return]()
					{
						affix_base::data::byte_buffer l_serialized_return = a_get_return();

						if constexpr (std::is_same<RETURN_TYPE, void>::value)
						{
							return;
						}
						else
						{
							RETURN_TYPE l_deserialized_result;
							l_serialized_return.pop_front(l_deserialized_result);
							return l_deserialized_result;
						}

					}));

			}

		};

	}
}
