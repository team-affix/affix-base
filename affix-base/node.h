#pragma once
#include "pch.h"
#include "ptr.h"

using affix_base::data::ptr;

namespace affix_base {
	namespace data {
		template<class T>
		class node {
		public:
			node() {

			}
			node(T a_val) {
				m_val = new T(a_val);
			}
			
		public:
			T& val() {
				return m_val.val();
			}
			node<T>& next() {
				return m_next.val();
			}
			node<T>& prev() {
				return m_prev.val();
			}
			node<T>& fwd(size_t a_i) {
				if (a_i <= 0)
					return *this;
				else
					return next().fwd(a_i - 1);
			}
			node<T>& bwd(size_t a_i) {
				if (a_i <= 0)
					return *this;
				else
					return prev().bwd(a_i - 1);
			}

		public:
			void insert(size_t a_i, node<T> a_val) {
				node<T>& before = fwd(a_i - 1);
				node<T>& after = fwd(a_i);
				ptr<node<T>> pointer = new node<T>(a_val);
				if (&before != nullptr)
					before.m_next.link(pointer);
				if (&after != nullptr)
					after.m_prev.link(pointer);
			}

		private:
			ptr<T> m_val = nullptr;
			ptr<node<T>> m_prev = nullptr;
			ptr<node<T>> m_next = nullptr;
		};
	}
}