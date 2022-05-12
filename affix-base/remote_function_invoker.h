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
			affix_base::data::byte_buffer serialize_function_args(
				SERIALIZED_PARAMETER_TYPES ... a_params
			)
			{
				affix_base::data::byte_buffer l_result;

				if constexpr (sizeof...(SERIALIZED_PARAMETER_TYPES) > 0)
				{
					l_result.push_back(a_params...);
				}

				return l_result;

			}

			template<typename RETURN_TYPE>
			std::future<RETURN_TYPE> async_deserialize_result(
				const std::function<affix_base::data::byte_buffer()>& a_get_bytes
			)
			{
				return std::async(std::launch::deferred,
					[&, a_get_bytes]()
					{
						affix_base::data::byte_buffer l_output = a_get_bytes();
						if constexpr (std::is_same<RETURN_TYPE, void>::value)
						{
							return;
						}
						else
						{
							RETURN_TYPE l_result;
							l_output.pop_front(l_result);
							return l_result;
						}
					});
			}

		};

	}
}
