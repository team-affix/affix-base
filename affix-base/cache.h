#pragma once
#include "pch.h"

namespace affix_base
{
	namespace data
	{
		template<typename T>
		class cache
		{
		protected:
			T m_resource;

		public:
			/// <summary>
			/// Attempts to import the resource. If the resource cannot be pulled, it will be initialized.
			/// </summary>
			/// <returns></returns>
			void import_resource(

			)
			{
				try
				{
					m_pull(m_resource);
					m_validate(m_resource);
				}
				catch (std::exception a_exception)
				{
					m_import_failed_callback(m_resource, a_exception);
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
					m_validate(m_resource);
					m_push(m_resource);
				}
				catch (std::exception a_exception)
				{
					m_export_failed_callback(m_resource, a_exception);
				}
			}

		protected:
			/// <summary>
			/// Pulls from the remote.
			/// </summary>
			/// <returns></returns>
			std::function<void(T&)> m_pull = [](T&) {};

			/// <summary>
			/// Pushes to the remote.
			/// </summary>
			/// <returns></returns>
			std::function<void(T&)> m_push = [](T&) {};

			/// <summary>
			/// Validates that the local is in an acceptable form.
			/// </summary>
			/// <returns></returns>
			std::function<void(T&)> m_validate = [](T&) {};

		protected:
			/// <summary>
			/// Callback for when importing fails.
			/// </summary>
			std::function<void(T&, std::exception)> m_import_failed_callback = [](T&, std::exception a_exception) { throw a_exception; };

			/// <summary>
			/// Callback for when exporting fails.
			/// </summary>
			std::function<void(T&, std::exception)> m_export_failed_callback = [](T&, std::exception a_exception) { throw a_exception; };

		public:
			void set_pull(
				const std::function<void(T&)>& a_pull
			)
			{
				m_pull = a_pull;
			}

			void set_push(
				const std::function<void(T&)>& a_push
			)
			{
				m_push = a_push;
			}

			void set_validate(
				const std::function<void(T&)>& a_validate
			)
			{
				m_validate = a_validate;
			}

			void set_import_failed_callback(
				const std::function<void(T&, std::exception)>& a_import_failed_callback
			)
			{
				m_import_failed_callback = a_import_failed_callback;
			}

			void set_export_failed_callback(
				const std::function<void(T&, std::exception)>& a_export_failed_callback
			)
			{
				m_import_failed_callback = a_export_failed_callback;
			}

			T& resource(

			)
			{
				return m_resource;
			}
			
		};
	}
}
