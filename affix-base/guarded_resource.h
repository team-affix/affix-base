#pragma once
#include "pch.h"

namespace affix_base
{
	namespace threading
	{
		template<typename RESOURCE_TYPE, typename MUTEX_TYPE>
		struct locked_resource
		{
		protected:
			RESOURCE_TYPE& m_resource;
			std::lock_guard<MUTEX_TYPE> m_lock_guard;

		public:
			locked_resource(
				RESOURCE_TYPE& a_resource,
				MUTEX_TYPE& a_mutex
			) :
				m_resource(a_resource),
				m_lock_guard(a_mutex)
			{

			}

			RESOURCE_TYPE& resource(

			)
			{
				return m_resource;
			}

			RESOURCE_TYPE* operator ->()
			{
				return &resource();
			}

			RESOURCE_TYPE& operator*()
			{
				return resource();
			}

		};

		template<typename RESOURCE_TYPE, typename MUTEX_TYPE>
		class guarded_resource
		{
		protected:
			RESOURCE_TYPE m_resource;
			MUTEX_TYPE m_mutex;

		public:
			guarded_resource(

			)
			{

			}
			guarded_resource(
				const RESOURCE_TYPE& a_resource
			) :
				m_resource(a_resource)
			{

			}

			locked_resource<RESOURCE_TYPE, MUTEX_TYPE> lock(
				
			)
			{
				return locked_resource<RESOURCE_TYPE, MUTEX_TYPE>(m_resource, m_mutex);
			}

		};
	}
}
