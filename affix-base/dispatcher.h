#pragma once
#include "pch.h"
#include "cross_thread_mutex.h"
#include "guarded_resource.h"

namespace affix_base
{
	namespace callback
	{
		/// <summary>
		/// A templated class which holds information regarding the dispatched state of a callback function.
		/// </summary>
		/// <typeparam name="MUTEX_TYPE"></typeparam>
		/// <typeparam name="RETURN_TYPE"></typeparam>
		/// <typeparam name="...PARAMETER_TYPES"></typeparam>
		template<typename MUTEX_TYPE, typename RETURN_TYPE = void, typename ... PARAMETER_TYPES>
		class dispatcher
		{
		protected:
			/// <summary>
			/// The current number of dispatched callbacks.
			/// </summary>
			affix_base::threading::guarded_resource<size_t, MUTEX_TYPE> m_dispatched_count = 0;

		public:
			/// <summary>
			/// Returns a tracking function for an internal function.
			/// </summary>
			/// <typeparam name="MUTEX_TYPE"></typeparam>
			/// <typeparam name="INT_TYPE"></typeparam>
			/// <typeparam name="RETURN_TYPE"></typeparam>
			/// <typeparam name="...PARAMETER_TYPES"></typeparam>
			std::function<RETURN_TYPE(PARAMETER_TYPES...)> dispatch(
				const std::function<RETURN_TYPE(PARAMETER_TYPES...)>& a_function
			)
			{
				// Lock the guarded_resource, preventing concurrent reads/writes to the dispatch count
				affix_base::threading::locked_resource<size_t> l_locked_resource_0 = m_dispatched_count.lock();
				l_locked_resource_0.resource()++;

				if constexpr (std::is_same<RETURN_TYPE, void>::value)
				{
					// Function has a return type of void

					if constexpr (sizeof...(PARAMETER_TYPES) == 0)
					{
						// Function takes zero parameters

						return [&, a_function]()
						{
							// Call the embedded function
							a_function();

							// Lock the guarded_resource, preventing concurrent reads/writes to the dispatch count
							affix_base::threading::locked_resource<size_t> l_locked_resource_1 = m_dispatched_count.lock();
							l_locked_resource_1.resource()--;

						};

					}
					else
					{
						// Function takes some non-zero number of parameters

						return [&, a_function](PARAMETER_TYPES&&... a_args)
						{
							// Call the embedded function
							a_function(std::forward<PARAMETER_TYPES>(a_args)...);

							// Lock the guarded_resource, preventing concurrent reads/writes to the dispatch count
							affix_base::threading::locked_resource<size_t> l_locked_resource_1 = m_dispatched_count.lock();
							l_locked_resource_1.resource()--;

						};

					}

				}
				else
				{
					// Function has a non-void return type

					if constexpr (sizeof...(PARAMETER_TYPES) == 0)
					{
						// Function takes zero parameters.

						return [&, a_function]()
						{
							// Call the embedded function
							RETURN_TYPE l_result = a_function();

							// Lock the guarded_resource, preventing concurrent reads/writes to the dispatch count
							affix_base::threading::locked_resource<size_t> l_locked_resource_1 = m_dispatched_count.lock();
							l_locked_resource_1.resource()--;

							return std::forward<RETURN_TYPE>(l_result);

						};

					}
					else
					{
						// Function has some non-zero number of parameters

						return [&, a_function](PARAMETER_TYPES&&... a_args)
						{
							// Call the embedded function
							RETURN_TYPE l_result = a_function(std::forward<PARAMETER_TYPES>(a_args)...);

							// Lock the guarded_resource, preventing concurrent reads/writes to the dispatch count
							affix_base::threading::locked_resource<size_t> l_locked_resource_1 = m_dispatched_count.lock();
							l_locked_resource_1.resource()--;

							return std::forward<RETURN_TYPE>(l_result);

						};

					}
				}
			}

			/// <summary>
			/// Returns the number of dispatches currently active.
			/// </summary>
			/// <returns></returns>
			affix_base::threading::locked_resource<size_t> dispatched_count(

			)
			{
				return m_dispatched_count.lock();
			}

		};
	}
}
