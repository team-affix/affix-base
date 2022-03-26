#pragma once
#include "pch.h"
#include "byte_buffer.h"

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
						return a_byte_buffer.push_back(a_fields...);
					}),
				m_deserialize(
					[&](affix_base::data::byte_buffer& a_byte_buffer)
					{
						return a_byte_buffer.pop_front(a_fields...);
					})
			{

			}

			serializable(
				const std::function<bool(affix_base::data::byte_buffer&)>& a_serialize,
				const std::function<bool(affix_base::data::byte_buffer&)>& a_deserialize
			) :
				m_serialize(a_serialize),
				m_deserialize(a_deserialize)
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

