#pragma once
#include "pch.h"
#include "cross_thread_mutex.h"

namespace affix_base
{
	namespace threading
	{
		template<typename RESOURCE_TYPE>
		class guarded_resource
		{
		protected:
			cross_thread_mutex m_resource_mutex;
			RESOURCE_TYPE m_resource;

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

			void lock(

			)
			{
				while (!try_lock());
			}

			bool try_lock(
				
			)
			{
				return m_resource_mutex.try_lock();
			}

			void unlock(

			)
			{
				m_resource_mutex.unlock();
			}

			RESOURCE_TYPE& resource(

			)
			{
				if (m_resource_mutex.id() != std::this_thread::get_id())
					throw std::exception("[ GUARDED RESOURCE ] Error: unable to obtain resource as it is not owned by the current thread. ");
				return m_resource;
			}

			RESOURCE_TYPE& operator*(

			)
			{
				return resource();
			}

			RESOURCE_TYPE* operator->(

			)
			{
				return &resource();
			}

		};
	}
}
