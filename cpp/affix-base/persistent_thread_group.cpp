#include "persistent_thread_group.h"

using affix::threading::persistent_thread_group;

persistent_thread_group::persistent_thread_group() {

}

persistent_thread_group::persistent_thread_group(vector<persistent_thread> a_vec) : vector<persistent_thread>(a_vec) {

}

void persistent_thread_group::compile() {
	for (int i = 0; i < size(); i++) {
		at(i).m_continue.link(m_continue);
		at(i).m_executing.link(m_executing);
	}
}

void persistent_thread_group::execute() {
	m_executing.val() = true;
}

void persistent_thread_group::join() {
	for (int i = 0; i < size(); i++)
		at(i).join();
}