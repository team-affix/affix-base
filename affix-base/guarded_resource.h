#pragma once
#include "pch.h"
#include "cross_thread_mutex.h"

namespace affix_base
{
	namespace threading
	{
		template<typename RESOURCE_TYPE>
		struct locked_resource
		{
		protected:
			RESOURCE_TYPE& m_resource;
			uint64_t m_mutation_index;
			std::function<void()> m_on_deconstruct_callback;

		public:
			virtual ~locked_resource(

			)
			{
				m_on_deconstruct_callback();
			}

			locked_resource(
				RESOURCE_TYPE& a_resource,
				const uint64_t& a_mutation_index,
				const std::function<void()>& a_on_deconstruct_callback = [] {}
			) :
				m_resource(a_resource),
				m_mutation_index(a_mutation_index),
				m_on_deconstruct_callback(a_on_deconstruct_callback)
			{

			}

			locked_resource(
				const locked_resource<RESOURCE_TYPE>&
			) = delete;
			
			locked_resource& operator=(
				const locked_resource&
			) = delete;

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

			const uint64_t& mutation_index(

			) const
			{
				return m_mutation_index;
			}

		};
		template<typename RESOURCE_TYPE>
		struct const_locked_resource
		{
		protected:
			RESOURCE_TYPE& m_resource;
			uint64_t m_mutation_index;
			std::function<void()> m_on_deconstruct_callback;

		public:
			virtual ~const_locked_resource(

			)
			{
				m_on_deconstruct_callback();
			}

			const_locked_resource(
				RESOURCE_TYPE& a_resource,
				const uint64_t& a_mutation_index,
				const std::function<void()>& a_on_deconstruct_callback = [] {}
			) :
				m_resource(a_resource),
				m_mutation_index(a_mutation_index),
				m_on_deconstruct_callback(a_on_deconstruct_callback)
			{

			}

			const_locked_resource(
				const const_locked_resource<RESOURCE_TYPE>&
			) = delete;

			const_locked_resource& operator=(
				const const_locked_resource&
			) = delete;

			const RESOURCE_TYPE& resource(

			) const
			{
				return m_resource;
			}

			const RESOURCE_TYPE* operator ->(

				) const
			{
				return &resource();
			}

			const RESOURCE_TYPE& operator*(

				) const
			{
				return resource();
			}

			const uint64_t& mutation_index(

			) const
			{
				return m_mutation_index;
			}

		};

		template<typename RESOURCE_TYPE, typename MUTEX_TYPE = affix_base::threading::cross_thread_mutex>
		class guarded_resource
		{
		protected:
			RESOURCE_TYPE m_resource;
			MUTEX_TYPE m_mutex;


			uint64_t m_mutation_index = 0;

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

			virtual locked_resource<RESOURCE_TYPE> lock(
				
			)
			{
				// Lock the internal mutex now
				m_mutex.lock();

				// Increment the mutation index immediately
				m_mutation_index++;

				return locked_resource<RESOURCE_TYPE>(m_resource, m_mutation_index,
					[&]
					{
						// Unlock the internal mutex on deconstructor callback
						m_mutex.unlock();
					});
			}

			virtual const_locked_resource<RESOURCE_TYPE> const_lock(

			)
			{
				// Lock the internal mutex now
				m_mutex.lock();

				return const_locked_resource<RESOURCE_TYPE>(m_resource, m_mutation_index,
					[&]
					{
						// Unlock the internal mutex on deconstructor callback
						m_mutex.unlock();
					});
			}

		};
	}
}
