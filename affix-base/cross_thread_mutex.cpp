#include "pch.h"
#include "cross_thread_mutex.h"
#include <iostream>

#if 0
#define LOG(x) std::clog << x << std::endl;
#else
#define LOG(x)
#endif

using affix_base::threading::cross_thread_mutex;
using std::lock_guard;

cross_thread_mutex::~cross_thread_mutex() {

}

void cross_thread_mutex::lock() {
	while (!try_lock());
}

bool cross_thread_mutex::try_lock()
{
	std::thread::id l_this_thread_id = std::this_thread::get_id();

	if (!m_state_mutex.try_lock())
		return false;

	std::thread::id l_id = m_id;
	bool l_mutex_already_owned = l_this_thread_id == l_id;

	// INCREMENT LOCK (ON SAME THREAD) INDEX
	if (l_mutex_already_owned) {
		m_lock_index++;
		LOG("[CROSS-THREAD-MUTEX] (" << l_this_thread_id << ") Incrementing lock index to (" << m_lock_index << ")." << std::endl);
	}

	m_state_mutex.unlock();

	if (!l_mutex_already_owned) {

		LOG("[CROSS-THREAD-MUTEX] (" << l_this_thread_id << ") Attempting to acquire internal mutex." << std::endl);
		if (!m_mutex.try_lock())
			return false;

		LOG(std::clog << "[CROSS-THREAD-MUTEX] (" << l_this_thread_id << ") Acquired internal mutex." << std::endl);
		m_id = l_this_thread_id;
		m_lock_index = 0;

		return true;
	}

	return true;

}

void cross_thread_mutex::unlock() {

	std::thread::id l_this_thread_id = std::this_thread::get_id();
	m_state_mutex.lock();
	
	// MUTEX MUST BE UNLOCKED SAME # OF TIMES IT WAS LOCKED
	if (m_lock_index > 0) {
		m_lock_index--;
		LOG("[CROSS-THREAD-MUTEX] (" << l_this_thread_id << ") Decrementing lock index to (" << m_lock_index << ")." << std::endl);
		m_state_mutex.unlock();
		return;
	}

	// CLEAR OWNER ID FIRST, BEFORE INTERNAL MUTEX UNLOCK
	m_id = std::thread::id();

	LOG("[CROSS-THREAD-MUTEX] (" << l_this_thread_id << ") Unlocking internal mutex." << std::endl);
	m_mutex.unlock();

	m_state_mutex.unlock();

}

std::thread::id cross_thread_mutex::id()
{
	std::lock_guard l_lock(m_state_mutex);
	return m_id;
}
