#pragma once
#include "pch.h"
#include <map>
#define PTR_DEBUG 0

using std::shared_ptr;
using std::map;

namespace affix_base {
	namespace data {

		class ptr_base {
		protected:
			static map<void*, ptr_base*> res_map;

		public:
			ptr_base* m_next = nullptr;
			ptr_base* m_prev = nullptr;

		};

		template<typename T>
		class ptr : public ptr_base {
		public:
			mutable T* m_raw = nullptr;

		public:
			virtual ~ptr() {
				unlink();
			}
			ptr() {

			}
			ptr(T* a_raw) {
				link(a_raw);
			}
			void operator=(T* a_raw) {
				link(a_raw);
			}
			template<typename J>
			ptr(J* a_raw) {
				link((T*)a_raw);
			}
			void operator=(ptr<T> a_other) {
				link(a_other.m_raw);
			}
			template<typename J>
			ptr(ptr<J> a_other) {
				link((T*)a_other.m_raw);
			}
			ptr(const ptr<T>& a_other) {
				link(a_other.m_raw);
			}

		public:
			T* get() const {
				return m_raw;
			}
			T& val() {
				return *get();
			}
			T val() const {
				assert(m_raw != nullptr);
				return *m_raw;
			}
			operator T* () {
				return get();
			}
			template<typename J>
			operator J* () {
				return (J*)m_raw;
			}
			T* operator->() {
				return get();
			}

		public:
			void link(const ptr<T>& a_other) {
				link(a_other.m_raw);
			}
			void link(T* a_raw) {
				// LEAVE ANY RESOURCE GROUPS
				unlink();

				if (a_raw == nullptr)
					return;

				// CHECK MAP FOR PRE-OWNED MEMORY ADDRESS
				map<void*, ptr_base*>::iterator res_pair = res_map.find(a_raw);
				if (res_pair != res_map.end()) {
#if PTR_DEBUG
					std::cout << "GROUP JOIN" << std::endl;
#endif
					// JOIN PREEXISTING GROUP
					group_join(res_pair->second);
				}
				else {
#if PTR_DEBUG
					std::cout << "GROUP CREATE" << std::endl;
#endif
					// CREATE NEW GROUP FOR THIS RESOURCE
					res_map.insert({ a_raw, this });
				}

				// ACQUIRE RESOURCE
				m_raw = a_raw;
			}
			void unlink() {
				// CHECK MAP FOR OWNERSHIP OVER A RESOURCE
				map<void*, ptr_base*>::iterator res_pair = res_map.find(m_raw);
				if (m_raw != nullptr && res_pair != res_map.end() && res_pair->second == this)
					if (m_prev != nullptr) {
#if PTR_DEBUG
						std::cout << "PASS OWNERSHIP TO PREV" << std::endl;
#endif
						// PASS OWNERSHIP TO m_prev
						res_pair->second = m_prev;
					}
					else if (m_next != nullptr) {
#if PTR_DEBUG
						std::cout << "PASS OWNERSHIP TO NEXT" << std::endl;
#endif
						// PASS OWNERSHIP TO m_next
						res_pair->second = m_next;
					}
					else {
#if PTR_DEBUG
						std::cout << "DELETE RESOURCE" << std::endl;
#endif
						// DELETE RESOURCE
						res_map.erase(res_pair);
						delete m_raw;
					}

				//std::cout << "GROUP LEAVE" << std::endl;
				group_leave();

				// CLEAR RESOURCE
				m_raw = nullptr;
			}

		protected:
			void group_join(ptr_base* a_other) {
				// NEVER LINK TO SELF
				if (a_other == (ptr_base*)this)
					return;

				if (a_other->m_next != nullptr)
					a_other->m_next->m_prev = this;
				m_next = a_other->m_next;
				a_other->m_next = this;
				m_prev = a_other;
			}
			void group_leave() {
				if (m_prev != nullptr)
					m_prev->m_next = m_next;
				if (m_next != nullptr)
					m_next->m_prev = m_prev;
				m_prev = nullptr;
				m_next = nullptr;
			}

		};

	}
}