#pragma once
#include "pch.h"
#include "byte_buffer.h"

namespace affix_base
{
	namespace distributed_computing
	{
		template<typename FUNCTION_IDENTIFIER_TYPE, typename INVOCATION_IDENTIFIER_TYPE, typename ... UNSERIALIZED_PARAMETER_TYPES>
		class remote_function_invoker
		{
		protected:
			std::function<INVOCATION_IDENTIFIER_TYPE(UNSERIALIZED_PARAMETER_TYPES ..., FUNCTION_IDENTIFIER_TYPE, affix_base::data::byte_buffer)> m_call_function;
			std::function<affix_base::data::byte_buffer(INVOCATION_IDENTIFIER_TYPE)> m_get_result;

		public:
			remote_function_invoker(
				const std::function<INVOCATION_IDENTIFIER_TYPE(UNSERIALIZED_PARAMETER_TYPES ..., FUNCTION_IDENTIFIER_TYPE, affix_base::data::byte_buffer)>& a_call_function,
				const std::function<affix_base::data::byte_buffer(INVOCATION_IDENTIFIER_TYPE)>& a_get_result
			) :
				m_call_function(a_call_function),
				m_get_result(a_get_result)
			{

			}

			template<typename RETURN_TYPE, typename ... SERIALIZED_PARAMETER_TYPES>
			std::future<RETURN_TYPE> invoke(
				UNSERIALIZED_PARAMETER_TYPES ... a_unserialized_params,
				FUNCTION_IDENTIFIER_TYPE a_function_identifier,
				SERIALIZED_PARAMETER_TYPES ... a_params
			)
			{
				affix_base::data::byte_buffer l_input;

				if constexpr (sizeof...(SERIALIZED_PARAMETER_TYPES) > 0)
				{
					affix_base::data::serializable l_serializable(a_params...);
					l_serializable.serialize(l_input);
				}

				INVOCATION_IDENTIFIER_TYPE l_call_identifier = m_call_function(a_unserialized_params..., a_function_identifier, l_input);

				return std::async(std::launch::deferred,
					[&](INVOCATION_IDENTIFIER_TYPE a_call_identifier)
					{
						affix_base::data::byte_buffer l_output = m_get_result(a_call_identifier);
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
					}, l_call_identifier);

			}

		};

	}
}
