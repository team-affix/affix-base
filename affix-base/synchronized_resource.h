#pragma once
#include "pch.h"

namespace affix_base
{
	namespace data
	{
		class synchronized_resource
		{
		public:
			/// <summary>
			/// Attempts to import the resource. If the resource cannot be pulled, it will be initialized.
			/// </summary>
			/// <returns></returns>
			bool import_resource(

			)
			{
				if (pull())
				{
					return validate();
				}
				else
				{
					return initialize();
				}
			}

			/// <summary>
			/// Attempts to export the resource. If the resource can be validated, it will be pushed.
			/// </summary>
			/// <returns></returns>
			bool export_resource(

			)
			{
				if (validate())
				{
					return push();
				}
				else
				{
					return false;
				}
			}

		protected:
			/// <summary>
			/// Pulls from the remote.
			/// </summary>
			/// <returns></returns>
			virtual bool pull(

			)
			{
				return false;
			}

			/// <summary>
			/// Initializes the resource to something completely new.
			/// </summary>
			/// <returns></returns>
			virtual bool initialize(

			)
			{
				return false;
			}

			/// <summary>
			/// Validates that the resource is in an acceptable form.
			/// </summary>
			/// <returns></returns>
			virtual bool validate(

			)
			{
				return false;
			}

			/// <summary>
			/// Pushes to the remote.
			/// </summary>
			/// <returns></returns>
			virtual bool push(

			)
			{
				return false;
			}

		};
	}
}
