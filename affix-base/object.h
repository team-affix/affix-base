#pragma once
#include "pch.h"
#include "ptr.h"

namespace affix_base {
	namespace data {

		class object_base {
		protected:
			void* m_ptr = nullptr;
			std::function<void()> m_delete = nullptr;

		public:
			virtual ~object_base() {
				if (m_delete != nullptr)
					m_delete();
			}
			object_base() {

			}
			template<typename T>
			object_base(T* a_ptr) : m_ptr(a_ptr) {
				m_delete = [a_ptr] {
					delete a_ptr;
				};
			}

		public:
			void* get() const {
				return m_ptr;
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
			template<typename T>
			operator T* () {
				return (T*)m_ptr->get();
			}
			template<typename T>
			operator T& () {
				return *(T*)m_ptr->get();
			}

		};


	}
}
