#pragma once
#include "pch.h"
#include "byte_buffer.h"
#include "ptr.h"
#include "object.h"

namespace affix_base
{
	namespace distributed_computing
	{
		template<typename FUNCTION_IDENTIFIER_TYPE>
		class remote_function_invoker
		{
		public:
			template<typename ... SERIALIZED_PARAMETER_TYPES>
			bool serialize_invocation(
				affix_base::data::byte_buffer& a_output,
				const FUNCTION_IDENTIFIER_TYPE& a_function_identifier,
				SERIALIZED_PARAMETER_TYPES... a_serialized_args
			)
			{
				// Push function identifier to byte buffer.
				if (!a_output.push_back(a_function_identifier))
					return false;

				// Push function args to byte buffer.
				if constexpr (sizeof...(SERIALIZED_PARAMETER_TYPES) > 0)
				{
					if (!a_output.push_back(a_serialized_args...))
						return false;
				}

				return true;

			}

		};

	}
}
