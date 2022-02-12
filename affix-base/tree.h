#pragma once
#include "pch.h"

namespace affix_base
{
	namespace data
	{
		template<typename T>
		class tree : public std::vector<tree<T>>
		{
		public:
			class path : public std::vector<tree<T>*>
			{
			public:
				/// <summary>
				/// Default constructor, initializes the base vector to its default value.
				/// </summary>
				path(

				)
				{

				}

				/// <summary>
				/// Value-initializing constructor. Initializes the base vector to the argued value.
				/// </summary>
				/// <param name="a_subtrees"></param>
				path(
					const std::vector<tree<T>*>& a_subtrees
				) :
					std::vector<tree<T>*>(a_subtrees)
				{

				}

				/// <summary>
				/// Suggests the validity of the tree path. If a path is size==0, the path is considered invalid.
				/// </summary>
				/// <returns></returns>
				bool valid(

				) const
				{
					return std::vector<tree<T>*>::size() != 0;
				}

			};

		public:
			/// <summary>
			/// The resource which this node holds.
			/// </summary>
			T m_resource;

		public:
			/// <summary>
			/// Default constructor. Initializes m_resource to its default value.
			/// </summary>
			tree(

			)
			{

			}

			/// <summary>
			/// Value-initializing constructor. Initializes m_resource to the argued value.
			/// </summary>
			/// <param name="a_resource"></param>
			tree(
				const T& a_resource
			) :
				m_resource(a_resource)
			{

			}

			/// <summary>
			/// Value-initializing constructor. Initializes m_resource and the base vector to their argued values.
			/// </summary>
			/// <param name="a_resource"></param>
			/// <param name="a_subtrees"></param>
			tree(
				const T& a_resource,
				const std::vector<tree<T>>& a_subtrees
			) :
				m_resource(a_resource),
				std::vector<tree<T>>(a_subtrees)
			{

			}

			/// <summary>
			/// Returns a path to the node where the argued predicate is satisfied.
			/// </summary>
			/// <param name="a_predicate"></param>
			/// <returns></returns>
			path find_where(
				const std::function<bool(const T&)>& a_predicate
			)
			{
				// The path to the found node
				path l_path;

				// Check if this node matches.
				if (a_predicate(m_resource))
				{
					l_path.push_back(this);
					return l_path;
				}

				// Search subtrees for a match.
				for (int i = 0; i < std::vector<tree<T>>::size(); i++)
				{
					l_path = std::vector<tree<T>>::at(i).find_where(a_predicate);
					if (l_path.size() != 0)
					{
						l_path.insert(l_path.begin(), this);
						return l_path;
					}
				}

				// Under no subtrees did the query match.
				return l_path;

			}

			/// <summary>
			/// Returns a path to the node where the stored resource matches the argued resource.
			/// </summary>
			/// <param name="a_resource"></param>
			/// <returns></returns>
			path find(
				const T& a_resource
			)
			{
				return find_where(
					[&](const T& a_lambda_resource)
					{
						return a_lambda_resource == a_resource;
					});
			}

			/// <summary>
			/// Suggests whether the argued resource can be found within the tree.
			/// </summary>
			/// <param name="a_resource"></param>
			/// <returns></returns>
			bool contains(
				const T& a_resource
			)
			{
				return find(a_resource).valid();
			}

			/// <summary>
			/// Recursively applies a function to each node in the tree, using pre-order traversal.
			/// </summary>
			/// <param name="a_function"></param>
			void pre_order_traverse(
				const std::function<void(tree<T>&)>& a_function
			)
			{
				a_function(*this);
				for (int i = 0; i < std::vector<tree<T>>::size(); i++)
					std::vector<tree<T>>::at(i).pre_order_traverse(a_function);
			}

			/// <summary>
			/// Recursively applies a function to each node in the tree, using pre-order traversal, where each function call is supplied the node depth.
			/// </summary>
			/// <param name="a_function"></param>
			void pre_order_traverse(
				const std::function<void(tree<T>&, size_t)>& a_function,
				const size_t a_depth = 0
			)
			{
				a_function(*this, a_depth);
				for (int i = 0; i < std::vector<tree<T>>::size(); i++)
					std::vector<tree<T>>::at(i).pre_order_traverse(a_function, a_depth + 1);
			}

			/// <summary>
			/// Recursively applies a function to each node in the tree, using post-order traversal.
			/// </summary>
			/// <param name="a_function"></param>
			void post_order_traverse(
				const std::function<void(tree<T>&)>& a_function
			)
			{
				for (int i = 0; i < std::vector<tree<T>>::size(); i++)
					std::vector<tree<T>>::at(i).post_order_traverse(a_function);
				a_function(*this);
			}

			/// <summary>
			/// Recursively applies a function to each node in the tree, using pre-order traversal, where each function call is supplied the node depth.
			/// </summary>
			/// <param name="a_function"></param>
			void post_order_traverse(
				const std::function<void(tree<T>&, size_t)>& a_function,
				const size_t a_depth = 0
			)
			{
				for (int i = 0; i < std::vector<tree<T>>::size(); i++)
					std::vector<tree<T>>::at(i).post_order_traverse(a_function, a_depth + 1);
				a_function(*this, a_depth);
			}

		};
	}
}
