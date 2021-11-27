#include "pch.h"
#include "watch.h"

using std::function;
using affix_base::callback::watch;

watch::watch(const function<bool()>& a_predicate, const function<void()>& a_callback) : m_predicate(a_predicate), m_callback(a_callback) {

}

void watch::process() {

	bool l_predicate_match = m_predicate();

	if (!m_predicate_match && l_predicate_match)
		m_callback();

	m_predicate_match = l_predicate_match;

}
