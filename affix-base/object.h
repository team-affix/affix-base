#pragma once
#include "pch.h"
#include "ptr.h"
#include "catch_friendly_assert.h"

namespace affix_base {
	namespace data {

		class object_base {
		protected:
			void* m_ptr = nullptr;
			std::function<void()> m_delete = nullptr;
			string m_type_name;

		public:
			virtual ~object_base() {
				if (m_delete != nullptr)
					m_delete();
			}
			object_base() {

			}
			template<typename T>
			object_base(T* a_ptr) : m_ptr(a_ptr), m_type_name(typeid(*a_ptr).name()) {
				m_delete = [a_ptr] {
					delete a_ptr;
				};
			}

		public:
			void* get() const {
				return m_ptr;
			}
			const string& type() const {
				return m_type_name;
			}

		};

		class object {
		protected:
			ptr<object_base> m_ptr = nullptr;

		public:
			object() {

			}
			template<typename T>
			object(T* a_ptr) : m_ptr(new object_base(a_ptr)) {

			}

		public:
			const string& type() {
				return m_ptr->type();
			}

		public:
			template<typename T>
			bool type_equals() {
				return m_ptr->type() == typeid(T).name();
			}

		public:
			template<typename T>
			operator T* () {
				CATCH_FRIENDLY_ASSERT(type_equals<T>(), "Types do not match: " + m_ptr->type() + ", and " + typeid(T).name());
				return (T*)m_ptr->get();
			}
			template<typename T>
			operator T& () {
				CATCH_FRIENDLY_ASSERT(type_equals<T>(), "Types do not match: " + m_ptr->type() + ", and " + typeid(T).name());
				return *(T*)m_ptr->get();
			}


		};


	}
}
