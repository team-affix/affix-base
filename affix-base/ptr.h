#pragma once
#include "pch.h"
#include <iostream>

using std::shared_ptr;

namespace affix_base {
	namespace data {

		class ptr_base {
		public:
			ptr_base() { m_next = nullptr; m_prev = nullptr; }
			ptr_base* m_next = nullptr;
			ptr_base* m_prev = nullptr;
		};

		template<typename T>
		class ptr : public ptr_base {
		protected:
			T* raw_ptr = nullptr;

		public:
			virtual ~ptr() {
				unlink();
			}
			ptr() {

			}
			ptr(T* a_raw_ptr) {
				raw_ptr = a_raw_ptr;
			}
			void operator=(T* a_raw_ptr) {
				unlink();
				raw_ptr = a_raw_ptr;
			}
			void operator=(ptr<T> a_other) {
				unlink();
				link(a_other);
			}
			ptr(const ptr<T>& a_other) {
				link(const_cast<ptr<T>&>(a_other));
			}
			template<typename J>
			ptr(const ptr<J>& a_other) {
				link(const_cast<ptr<J>&>(a_other));
			}

		public:
			T* get() {
				return raw_ptr;
			}
			T& val() {
				return *get();
			}
			T* operator->() {
				return get();
			}
			T& operator*() {
				return val();
			}
			template<typename J>
			operator J* () {
				return (J*)get();
			}

		public:
			template<typename J>
			void link(ptr<J>& a_other) {
				m_prev = &a_other;
				if (a_other.m_next != nullptr)
					a_other.m_next->m_prev = this;
				a_other.m_next = this;
				raw_ptr = (T*)a_other.get();
			}
			void unlink() {
				bool m_prev_null = m_prev == nullptr;
				bool m_next_null = m_next == nullptr;
				if (m_prev_null && m_next_null) {
					delete raw_ptr;
					raw_ptr = nullptr;
				}
				else {
					if (!m_prev_null) {
						//std::cout << m_prev->m_next << std::endl;
						m_prev->m_next = m_next;
					}
					if (!m_next_null)
						m_next->m_prev = m_prev;
					m_prev = nullptr;
					m_next = nullptr;
					raw_ptr = nullptr;
				}
			}

		};

	}
}