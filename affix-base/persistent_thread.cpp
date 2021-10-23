#include "pch.h"
#include "persistent_thread.h"

using affix_base::threading::persistent_thread;

persistent_thread::~persistent_thread() {
	m_persist.val() = false;
	m_thread.join();
	m_function = nullptr;
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
	m_function = a_other.m_function;
}

void persistent_thread::init() {
	m_thread = thread([&]() {
		while (m_persist.val())
			if (m_call.val()) {
				
				// ENABLE EXECUTION IN PROGRESS
				m_executing.val() = true;
				
				// DISABLE THE TRIGGER
				m_call.val() = false;
				
				// RUN FUNCTION
				m_function();

				// DISABLE EXECUTION IN PROGRESS
				m_executing.val() = false;

				if (m_loop.val())
					call();

			}
	});
}

void persistent_thread::call() {
	join_call();
	m_call.val() = true;
}

void persistent_thread::call(function<void()> a_func) {
	m_function = a_func;
	call();
}

void persistent_thread::loop() {
	join_call();
	m_loop.val() = true;
	call();
}

void persistent_thread::loop(function<void()> a_func) {
	m_function = a_func;
	loop();
}

void persistent_thread::stop_loop() {
	m_loop.val() = false;
	join_loop();
}

void persistent_thread::join_call() {
	while (m_call.val() || m_executing.val());
}

void persistent_thread::join_loop() {
	while (m_loop.val());
	join_call();
}