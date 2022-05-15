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
			template<typename ... SERIALIZED_PARAMETER_TYPES>
			affix_base::data::byte_buffer serialize_invocation(
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

				return l_serialized_function_call_byte_buffer;

			}

		};

	}
}
