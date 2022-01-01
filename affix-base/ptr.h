#pragma once
#include "pch.h"
#include <map>
#define PTR_DEBUG 0
#if PTR_DEBUG
#include <iostream>
#endif

namespace affix_base {
	namespace data {

		class ptr_base {
		protected:
			static std::map<void*, std::vector<ptr_base*>> s_res_map;

		public:
			void link(void* a_raw) {

				// LEAVE ANY RESOURCE GROUPS
				unlink();

				if (a_raw == nullptr)
					return;

				// CHECK MAP FOR PRE-OWNED MEMORY ADDRESS
				std::map<void*, std::vector<ptr_base*>>::iterator l_res_pair = s_res_map.find(a_raw);

				if (l_res_pair != s_res_map.end()) {

					std::vector<ptr_base*>& group = l_res_pair->second;
#if PTR_DEBUG
					std::cout << "GROUP JOIN" << std::endl;
#endif
					// JOIN PREEXISTING GROUP
					group.push_back(this);
				}
				else {
#if PTR_DEBUG
					std::cout << "GROUP CREATE" << std::endl;
#endif
					// CREATE NEW GROUP FOR THIS RESOURCE
					s_res_map.insert({ a_raw, { this } });
				}

				// ACQUIRE RESOURCE
				acquire_resource(a_raw);

			}
			void unlink() {

				if (!owns_resource())
					return;

				// CHECK MAP FOR OWNERSHIP OVER A RESOURCE
				std::map<void*, std::vector<ptr_base*>>::iterator l_res_pair = res_pair();

				if (l_res_pair != s_res_map.end()) {

					std::vector<ptr_base*>& group = l_res_pair->second;

					if (group.size() > 1) {
#if PTR_DEBUG
						std::cout << "LEAVE GROUP" << std::endl;
#endif
						// LEAVE GROUP
						std::vector<ptr_base*>::iterator this_iterator = std::find(group.begin(), group.end(), this);
						group.erase(this_iterator);
					}
					else {
#if PTR_DEBUG
						std::cout << "DELETE RESOURCE" << std::endl;
#endif
						// LEAVE GROUP AND DELETE RESOURCE
						s_res_map.erase(l_res_pair);
						delete_resource();
					}

				}

				// CLEAR RESOURCE
				clear_resource();

			}

		public:
			void group_link(
				void* a_raw
			)
			{
				std::map<void*, std::vector<ptr_base*>>::iterator l_res_pair = res_pair();

				std::vector<ptr_base*>& l_group = l_res_pair->second;

				// EACH MEMBER OF GROUP LINKS RESOURCE
				for (int i = l_group.size() - 1; i >= 0; i--)
					l_group[i]->link(a_raw);

			}
			void group_unlink()
			{
				std::map<void*, std::vector<ptr_base*>>::iterator l_res_pair = res_pair();

				std::vector<ptr_base*>& l_group = l_res_pair->second;

				// EACH MEMBER OF GROUP LINKS RESOURCE
				for (int i = l_group.size() - 1; i >= 0; i--)
					l_group[i]->unlink();

			}

		protected:
			virtual void acquire_resource(void* a_raw)
			{

			}
			virtual void delete_resource()
			{

			}
			virtual void clear_resource()
			{

			}
			virtual bool owns_resource()
			{
				return false;
			}
			virtual std::map<void*, std::vector<ptr_base*>>::iterator res_pair()
			{
				return s_res_map.end();
			}

		};

		template<typename T>
		class ptr : public ptr_base {
		public:
			mutable T* m_raw = nullptr;

		public:
			virtual ~ptr() {
				unlink();
			}

		public:
			ptr() {

			}
			ptr(void* a_raw) {
				link((T*)a_raw);
			}
			ptr(const ptr<T>& a_other) {
				link(a_other.m_raw);
			}
			template<typename J>
			ptr(const ptr<J>& a_other) {
				link((T*)a_other.m_raw);
			}

		public:
			ptr<T>& operator=(void* a_raw) {
				link((T*)a_raw);
				return *this;
			}
			ptr<T>& operator=(const ptr<T>& a_other) {
				link(a_other.m_raw);
				return *this;
			}
			template<typename J>
			ptr<T>& operator=(const ptr<J>& a_other) {
				link(a_other.m_raw);
				return *this;
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

		public:
			T* operator->() {
				return get();
			}
			const T* operator->() const
			{
				return get();
			}

		public:
			template<typename J>
			operator J* () const {
				return (J*)m_raw;
			}

		protected:
			virtual void acquire_resource(void* a_raw)
			{
				m_raw = (T*)a_raw;
			}
			virtual void delete_resource()
			{
				delete m_raw;
			}
			virtual void clear_resource()
			{
				m_raw = nullptr;
			}
			virtual bool owns_resource()
			{
				return m_raw != nullptr;
			}
			virtual std::map<void*, std::vector<ptr_base*>>::iterator res_pair()
			{
				return s_res_map.find(m_raw);
			}

		};

	}
}