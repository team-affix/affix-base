#pragma once
#include "pch.h"
#include "byte_buffer.h"

template<typename FUNCTION_TYPE, typename ... PARAMS>
constexpr void constexpr_for(FUNCTION_TYPE&& a_function, PARAMS&& ... a_params)
{
	(a_function(std::forward<PARAMS>(a_params)), ...);
}

namespace affix_base
{
	namespace data
	{
		class serializable
		{
		protected:
			std::function<bool(affix_base::data::byte_buffer&)> m_serialize;
			std::function<bool(affix_base::data::byte_buffer&)> m_deserialize;

		public:
			serializable(

			) = delete;

			template<typename ... FIELDS>
			serializable(
				FIELDS& ... a_fields
			) :
				m_serialize(
					[&](affix_base::data::byte_buffer& a_byte_buffer)
					{
						try
						{
							constexpr_for(
								[&](auto& a_field)
								{
									if (!a_byte_buffer.push_back(a_field))
										throw std::exception();
								}, a_fields...);
							return true;
						}
						catch (std::exception)
						{
							return false;
						}
					}),
				m_deserialize(
					[&](affix_base::data::byte_buffer& a_byte_buffer)
					{
						try
						{
							constexpr_for(
								[&](auto& a_field)
								{
									if (!a_byte_buffer.pop_front(a_field))
										throw std::exception();
								}, a_fields...);
							return true;
						}
						catch (std::exception)
						{
							return false;
						}
					})
			{

			}

			serializable(
				const serializable&
			) = delete;

			bool serialize(
				affix_base::data::byte_buffer& a_byte_buffer
			) const
			{
				return m_serialize(a_byte_buffer);
			}

			bool deserialize(
				affix_base::data::byte_buffer& a_byte_buffer
			)
			{
				return m_deserialize(a_byte_buffer);
			}


		};

	}
}

