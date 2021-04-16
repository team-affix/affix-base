#include "persistent_thread_group.h"

using affix_base::threading::persistent_thread_group;

persistent_thread_group::persistent_thread_group() {

}

persistent_thread_group::persistent_thread_group(vector<persistent_thread> a_vec) : vector<persistent_thread>(a_vec) {

}

void persistent_thread_group::compile() {
	for (int i = 0; i < size(); i++)
		at(i).m_execute_start.link(m_execute_start);
}

void persistent_thread_group::execute() {
	m_execute_start.val() = true;
}

void persistent_thread_group::join() {
	for (int i = 0; i < size(); i++)
		at(i).join();
}