#pragma once
#include "pch.h"
#include "byte_buffer.h"
#include "serializable.h"
#include "ptr.h"

namespace affix_base
{
	namespace distributed_computing
	{
		template<typename FUNCTION_IDENTIFIER_TYPE, typename ... PROCESSING_ASSISTANCE_TYPES>
		class remote_invocation_processor
		{
		protected:
			std::map<FUNCTION_IDENTIFIER_TYPE, std::function<void(PROCESSING_ASSISTANCE_TYPES..., affix_base::data::byte_buffer)>> m_function_map;

		public:
			template<typename ... PARAMETER_TYPES>
			void add_function(
				FUNCTION_IDENTIFIER_TYPE a_function_identifier,
				std::function<void(PROCESSING_ASSISTANCE_TYPES..., PARAMETER_TYPES ...)> a_function
			)
			{
				std::function<void(PROCESSING_ASSISTANCE_TYPES..., affix_base::data::byte_buffer)> l_serial_function =
					[&, a_function](PROCESSING_ASSISTANCE_TYPES... a_processing_assistance_args, affix_base::data::byte_buffer a_byte_buffer)
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
						if (!l_serializer->deserialize(a_byte_buffer))
							return;

						std::apply(
							[&](PARAMETER_TYPES&... a_params)
							{
								a_function(a_processing_assistance_args..., a_params...);
							}, l_tuple);

					}
					else
					{
						a_function(a_processing_assistance_args...);
					}

				};

				m_function_map.insert({ a_function_identifier, l_serial_function });

			}

			void remove_function(
				FUNCTION_IDENTIFIER_TYPE a_function_identifier
			)
			{
				auto l_it = m_function_map.find(a_function_identifier);
				if (l_it != m_function_map.end())
					m_function_map.erase(l_it);
			}

			void process(
				PROCESSING_ASSISTANCE_TYPES... a_processing_assistance_args,
				affix_base::data::byte_buffer l_input
			)
			{
				FUNCTION_IDENTIFIER_TYPE l_function_identifier;
				l_input.pop_front(l_function_identifier);

				auto l_it = m_function_map.find(l_function_identifier);

				if (l_it != m_function_map.end())
				{
					l_it->second(a_processing_assistance_args..., l_input);
				}

			}

		};
	}
}
