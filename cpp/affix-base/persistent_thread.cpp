#include "persistent_thread.h"

using affix::threading::persistent_thread;

persistent_thread::~persistent_thread() {
	m_continue = false;
	m_thread.join();
}

persistent_thread::persistent_thread() {
	init();
}

persistent_thread::persistent_thread(function<void()> a_func) {
	m_function = a_func;
	init();
}

void persistent_thread::init() {
	m_thread = thread([&]() {
		while (m_continue)
			if (m_executing) {
				m_function();
				m_executing = false;
			}
	});
}

void persistent_thread::execute() {
	join();
	m_executing = true;
}

void persistent_thread::execute(function<void()> a_func) {
	m_function = a_func;
	execute();
}

void persistent_thread::join() {
	while (m_executing);
}