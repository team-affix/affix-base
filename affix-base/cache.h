#pragma once
#include "pch.h"

namespace affix_base
{
	namespace data
	{
		template<typename LOCAL_TYPE>
		class cache
		{
		protected:
			LOCAL_TYPE m_local;

		public:
			virtual ~cache(

			)
			{

			}

			/// <summary>
			/// Constructor, which initializes pull/push/validate methods, as well as 
			/// callback functions for event failures.
			/// </summary>
			/// <param name="a_pull"></param>
			/// <param name="a_push"></param>
			/// <param name="a_validate"></param>
			/// <param name="a_local"></param>
			/// <param name="a_import_failed_callback"></param>
			/// <param name="a_export_failed_callback"></param>
			cache(
				const std::function<void(LOCAL_TYPE&)>& a_pull,
				const std::function<void(LOCAL_TYPE&)>& a_push,
				const std::function<void(LOCAL_TYPE&)>& a_validate,
				LOCAL_TYPE a_local = {},
				const std::function<void(LOCAL_TYPE&, std::exception a_exception)>& a_import_failed_callback = [](LOCAL_TYPE&, std::exception) {},
				const std::function<void(LOCAL_TYPE&, std::exception a_exceptions)>& a_export_failed_callback = [](LOCAL_TYPE&, std::exception) {}
			) :
				m_pull(a_pull),
				m_push(a_push),
				m_validate(a_validate),
				m_local(a_local),
				m_import_failed_callback(a_import_failed_callback),
				m_export_failed_callback(a_export_failed_callback)
			{

			}

			/// <summary>
			/// Attempts to import the resource. If the resource cannot be pulled, it will be initialized.
			/// </summary>
			/// <returns></returns>
			void import_resource(

			)
			{
				try
				{
					m_pull(m_local);
					m_validate(m_local);
				}
				catch (std::exception a_exception)
				{
					m_import_failed_callback(m_local, a_exception);
				}
			}

			/// <summary>
			/// Attempts to export the resource. If the resource can be validated, it will be pushed.
			/// </summary>
			/// <returns></returns>
			void export_resource(

			)
			{
				try
				{
					m_validate(m_local);
					m_push(m_local);
				}
				catch (std::exception a_exception)
				{
					m_export_failed_callback(m_local, a_exception);
				}
			}

		protected:
			/// <summary>
			/// Pulls from the remote.
			/// </summary>
			/// <returns></returns>
			std::function<void(LOCAL_TYPE&)> m_pull;

			/// <summary>
			/// Pushes to the remote.
			/// </summary>
			/// <returns></returns>
			std::function<void(LOCAL_TYPE&)> m_push;

			/// <summary>
			/// Validates that the local is in an acceptable form.
			/// </summary>
			/// <returns></returns>
			std::function<void(LOCAL_TYPE&)> m_validate;

		protected:
			/// <summary>
			/// Callback for when importing fails.
			/// </summary>
			std::function<void(LOCAL_TYPE&, std::exception)> m_import_failed_callback;

			/// <summary>
			/// Callback for when exporting fails.
			/// </summary>
			std::function<void(LOCAL_TYPE&, std::exception)> m_export_failed_callback;

		public:
			LOCAL_TYPE& local(

			)
			{
				return m_local;
			}
			
		};
	}
}
