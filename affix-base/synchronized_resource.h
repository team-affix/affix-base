#pragma once
#include "pch.h"

namespace affix_base
{
	namespace data
	{
		template<typename T>
		class synchronized_resource
		{
		protected:
			T m_resource;

		public:
			virtual ~synchronized_resource(

			)
			{

			}

			synchronized_resource(
				T a_resource
			) :
				m_resource(a_resource)
			{
				if (!import_resource())
				{
					if (initialize_resource())
					{
						if (export_resource())
						{

						}
						else
						{

						}
					}
					else
					{
						throw std::exception("Unable to initialize synchronized resource.");
					}
				}
			}

		public:
			virtual bool import_resource(

			)
			{
				return false;
			}

			virtual bool export_resource(

			)
			{
				return false;
			}

			virtual bool initialize_resource(

			)
			{
				return false;
			}

		public:
			virtual T& resource(

			)
			{
				return m_resource;
			}

		};
	}
}
