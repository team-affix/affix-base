#include "persistent_thread.h"

using affix::threading::persistent_thread;

persistent_thread::~persistent_thread() {
	m_continue.val() = false;
	m_thread.join();
}

persistent_thread::persistent_thread() {
	init();
}

persistent_thread::persistent_thread(function<void()> a_func) {
	m_function = a_func;
	init();
}

persistent_thread::persistent_thread(const persistent_thread& a_other) {
	operator=(a_other);
	init();
}

void persistent_thread::operator=(const persistent_thread& a_other) {
	m_continue = a_other.m_continue;
	m_execute_start = a_other.m_execute_start	;
	m_function = a_other.m_function;
}

void persistent_thread::init() {
	m_thread = thread([&]() {
		while (m_continue.val())
			if (m_executing.val()) {
				m_function();
				m_executing.val() = false;
			}
	});
}

void persistent_thread::execute() {
	join();
	m_executing.val() = true;
}

void persistent_thread::execute(function<void()> a_func) {
	m_function = a_func;
	execute();
}

void persistent_thread::join() {
	while (m_executing.val());
}