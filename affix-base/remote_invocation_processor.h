#pragma once
#include "pch.h"
#include "byte_buffer.h"
#include "serializable.h"

namespace affix_base
{
	namespace distributed_computing
	{
		template<typename FUNCTION_IDENTIFIER_TYPE, typename CALL_IDENTIFIER_TYPE>
		class remote_invocation_processor
		{
		protected:
			std::map<FUNCTION_IDENTIFIER_TYPE, std::function<affix_base::data::byte_buffer(affix_base::data::byte_buffer)>> m_function_map;

		public:
			template<typename RETURN_TYPE, typename ... PARAMETER_TYPES>
			void add_function(
				FUNCTION_IDENTIFIER_TYPE a_function_identifier,
				std::function<RETURN_TYPE(PARAMETER_TYPES ...)> a_function
			)
			{
				std::function<affix_base::data::byte_buffer(affix_base::data::byte_buffer)> l_serial_io_function =
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

				};

				m_function_map.insert({ a_function_identifier, l_serial_io_function });

			}

			void remove_function(
				FUNCTION_IDENTIFIER_TYPE a_function_identifier
			)
			{
				auto l_it = m_function_map.find(a_function_identifier);
				if (l_it != m_function_map.end())
					m_function_map.erase(l_it);
			}

		};
	}
}
