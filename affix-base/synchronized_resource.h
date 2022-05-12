#pragma once
#include "pch.h"
#include "guarded_resource.h"
#include "ptr.h"

namespace affix_base
{
	namespace threading
	{
		template<typename RESOURCE_TYPE, typename SYNCHRONIZED_RESOURCE_TYPE, typename MUTEX_TYPE = affix_base::threading::cross_thread_mutex>
		class synchronized_resource : public affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>
		{
		protected:
			affix_base::threading::guarded_resource<SYNCHRONIZED_RESOURCE_TYPE, MUTEX_TYPE>&
				m_synchronized_guarded_resource;

			std::function<void(const SYNCHRONIZED_RESOURCE_TYPE&, RESOURCE_TYPE&)> m_pull;
			std::function<void(const RESOURCE_TYPE&, SYNCHRONIZED_RESOURCE_TYPE&)> m_push;

		public:
			synchronized_resource(
				affix_base::threading::guarded_resource<SYNCHRONIZED_RESOURCE_TYPE, MUTEX_TYPE>& a_guarded_resource,
				const std::function<void(const SYNCHRONIZED_RESOURCE_TYPE&, RESOURCE_TYPE&)>& a_pull,
				const std::function<void(const RESOURCE_TYPE&, SYNCHRONIZED_RESOURCE_TYPE&)>& a_push
			) :
				m_synchronized_guarded_resource(a_guarded_resource),
				m_pull(a_pull),
				m_push(a_push)
			{
				// Initialize local resource by pulling
				const_locked_resource<SYNCHRONIZED_RESOURCE_TYPE> l_locked_resource = a_guarded_resource.const_lock();
				a_pull(l_locked_resource.resource(), affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource);
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index = 
					l_locked_resource.mutation_index();
			}

			synchronized_resource(
				affix_base::threading::guarded_resource<SYNCHRONIZED_RESOURCE_TYPE, MUTEX_TYPE>& a_guarded_resource,
				const std::function<void(const SYNCHRONIZED_RESOURCE_TYPE&, RESOURCE_TYPE&)>& a_pull,
				const std::function<void(const RESOURCE_TYPE&, SYNCHRONIZED_RESOURCE_TYPE&)>& a_push,
				const RESOURCE_TYPE& a_resource
			) :
				m_synchronized_guarded_resource(a_guarded_resource),
				m_pull(a_pull),
				m_push(a_push),
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>(a_resource)
			{
				// Initialize remote resource by pushing
				locked_resource<SYNCHRONIZED_RESOURCE_TYPE> l_locked_resource = a_guarded_resource.lock();
				a_push(affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource, l_locked_resource.resource());
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index =
					l_locked_resource.mutation_index();
			}

		public:
			virtual locked_resource<RESOURCE_TYPE> lock(

			)
			{
				// Lock the internal mutex now
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutex.lock();

				// Lock the synchronized resource
				affix_base::data::ptr<
					affix_base::threading::locked_resource<SYNCHRONIZED_RESOURCE_TYPE>> l_locked_resource = 
					new affix_base::threading::locked_resource<SYNCHRONIZED_RESOURCE_TYPE>(m_synchronized_guarded_resource.lock());

				// Increment the mutation index in hopes to match the synchronized mutation index
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index++;

				if (affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index != 
					l_locked_resource.val().mutation_index())
				{
					// The local data has fallen out of sync. It is time to pull
					m_pull(
						l_locked_resource.val().resource(),
						affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource);
				}

				// Resync mutation indices
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index = 
					l_locked_resource.val().mutation_index();
				
				return locked_resource<RESOURCE_TYPE>(
					affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource,
					affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index,
					[&, l_locked_resource]
					{
						// Push the data to the synchronized location
						m_push(
							affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource,
							(*l_locked_resource).resource());
						// Unlock the internal mutex on deconstructor callback
						affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutex.unlock();
					});
			}

			virtual const_locked_resource<RESOURCE_TYPE> const_lock(

			)
			{
				// Lock the internal mutex now
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutex.lock();

				// Lock the synchronized resource
				affix_base::data::ptr<
					affix_base::threading::const_locked_resource<SYNCHRONIZED_RESOURCE_TYPE>> l_locked_resource =
					new affix_base::threading::const_locked_resource<SYNCHRONIZED_RESOURCE_TYPE>(m_synchronized_guarded_resource.const_lock());

				if (affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index != 
					l_locked_resource.val().mutation_index())
				{
					// The local data has fallen out of sync. It is time to pull
					m_pull(
						l_locked_resource.val().resource(),
						affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource);
				}

				// Resync mutation indices
				affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index = 
					l_locked_resource.val().mutation_index();

				return const_locked_resource<RESOURCE_TYPE>(
					affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_resource,
					affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutation_index,
					[&, l_locked_resource]
					{
						// Unlock the internal mutex on deconstructor callback
						affix_base::threading::guarded_resource<RESOURCE_TYPE, MUTEX_TYPE>::m_mutex.unlock();
					});
			}


		};
		
	}
}