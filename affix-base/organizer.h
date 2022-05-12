#pragma once
#include "pch.h"

namespace affix_base
{
	namespace data
	{
		template<typename TAG_TYPE, typename ... PARAMETER_TYPES>
		class organizer
		{
		protected:
			std::map<TAG_TYPE, std::function<void(PARAMETER_TYPES...)>> m_filters;

		public:
			void insert_filter(
				const TAG_TYPE& a_tag,
				const std::function<void(PARAMETER_TYPES...)>& a_function
			)
			{
				auto l_iterator = m_filters.find(a_tag);
				
				if (l_iterator == m_filters.end())
				{
					m_filters.insert({ a_tag, a_function });
				}
				else
				{
					*l_iterator = a_function;
				}

			}

			bool remove_filter(
				const TAG_TYPE& a_tag
			)
			{
				auto l_iterator = m_filters.find(a_tag);

				if (l_iterator == m_filters.end())
					return false;

				m_filters.erase(l_iterator);

				return true;

			}

			bool process(
				const TAG_TYPE& a_tag,
				PARAMETER_TYPES... a_args
			)
			{
				auto l_iterator = m_filers.find(a_tag);

				if (l_iterator == m_filters.end())
					return false;

				l_iterator->second(a_args...);

				return true;

			}

		};
	}
}
