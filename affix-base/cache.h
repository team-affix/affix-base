#pragma once
#include "pch.h"
#include "utc_time.h"

namespace affix_base
{
	namespace data
	{
		template<typename T>
		class cache
		{
		protected:
			/// <summary>
			/// The local resource held in this object.
			/// </summary>
			T m_resource;

		protected:
			/// <summary>
			/// UTC time at which the resource was most recently imported.
			/// </summary>
			uint64_t m_last_imported_time = 0;

			/// <summary>
			/// UTC time at which the resource was most recently exported.
			/// </summary>
			uint64_t m_last_exported_time = 0;

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
					// Save the last imported time
					m_last_imported_time = affix_base::timing::utc_time();
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
					// Save the last exported time
					m_last_exported_time = affix_base::timing::utc_time();
				}
				catch (std::exception a_exception)
				{
					m_export_failed_callback(m_resource, a_exception);
				}
			}

			uint64_t last_imported_time(
				
			) const
			{
				return m_last_imported_time;
			}

			uint64_t last_exported_time(

			) const
			{
				return m_last_exported_time;
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
			/// <summary>
			/// Sets the pull function.
			/// </summary>
			/// <param name="a_pull"></param>
			void set_pull(
				const std::function<void(T&)>& a_pull
			)
			{
				m_pull = a_pull;
			}

			/// <summary>
			/// Sets the push function.
			/// </summary>
			/// <param name="a_push"></param>
			void set_push(
				const std::function<void(T&)>& a_push
			)
			{
				m_push = a_push;
			}

			/// <summary>
			/// Sets the validate function. (Responsible for validating the integrity of the resource.)
			/// </summary>
			/// <param name="a_validate"></param>
			void set_validate(
				const std::function<void(T&)>& a_validate
			)
			{
				m_validate = a_validate;
			}

			/// <summary>
			/// Sets the import_failed_callback function.
			/// </summary>
			/// <param name="a_import_failed_callback"></param>
			void set_import_failed_callback(
				const std::function<void(T&, std::exception)>& a_import_failed_callback
			)
			{
				m_import_failed_callback = a_import_failed_callback;
			}

			/// <summary>
			/// Sets the export_failed_callback function.
			/// </summary>
			/// <param name="a_export_failed_callback"></param>
			void set_export_failed_callback(
				const std::function<void(T&, std::exception)>& a_export_failed_callback
			)
			{
				m_import_failed_callback = a_export_failed_callback;
			}

			/// <summary>
			/// Gets the resource stored locally in the cache.
			/// </summary>
			/// <returns></returns>
			T& resource(

			)
			{
				return m_resource;
			}
			
		};
	}
}
