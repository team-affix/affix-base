#pragma once
#include "pch.h"
#include <map>
#define PTR_DEBUG 0

using std::shared_ptr;
using std::map;
using std::vector;

namespace affix_base {
	namespace data {

		class ptr_base {
		protected:
			static map<void*, vector<ptr_base*>> res_map;

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
			void operator=(const ptr<T>& a_other) {
				link(a_other.m_raw);
			}
			template<typename J>
			void operator=(const ptr<J>& a_other) {
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
				return *m_raw;
			}
			const T& val() const {
				return *m_raw;
			}
			operator T* () const {
				return get();
			}
			template<typename J>
			operator J* () const {
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
				map<void*, vector<ptr_base*>>::iterator res_pair = res_map.find(a_raw);

				if (res_pair != res_map.end()) {
					vector<ptr_base*>& group = res_pair->second;
					vector<ptr_base*>::iterator elem_this = std::find(group.begin(), group.end(), this);
					if (elem_this == group.end()) {
#if PTR_DEBUG
						std::cout << "GROUP JOIN" << std::endl;
#endif
						// JOIN PREEXISTING GROUP
						group.push_back(this);
					}
				}
				else {
#if PTR_DEBUG
					std::cout << "GROUP CREATE" << std::endl;
#endif
					// CREATE NEW GROUP FOR THIS RESOURCE
					res_map.insert({ a_raw, { this } });
			}

				// ACQUIRE RESOURCE
				m_raw = a_raw;
		}
			void unlink() {

				if (m_raw == nullptr)
					return;

				// CHECK MAP FOR OWNERSHIP OVER A RESOURCE
				map<void*, vector<ptr_base*>>::iterator res_pair = res_map.find(m_raw);

				if (res_pair != res_map.end()) {
					vector<ptr_base*>& group = res_pair->second;
					vector<ptr_base*>::iterator elem_this = std::find(group.begin(), group.end(), this);
					if (elem_this != group.end()) {
						if (group.size() > 1) {
#if PTR_DEBUG
							std::cout << "LEAVE GROUP" << std::endl;
#endif
							// PASS OWNERSHIP TO m_prev
							group.erase(elem_this);
						}
						else {
#if PTR_DEBUG
							std::cout << "DELETE RESOURCE" << std::endl;
#endif
							// DELETE RESOURCE
							res_map.erase(res_pair);
							delete m_raw;
				}
			}
	}

				// CLEAR RESOURCE
				m_raw = nullptr;
}

};

	}
}