#include "pch.h"
#include "persistent_thread.h"

using affix_base::threading::persistent_thread;
using std::function;
using std::thread;

persistent_thread::~persistent_thread(

)
{
	m_abort_thread = true;
	m_thread.join();
}

persistent_thread::persistent_thread(

) :
	m_owner_id(std::this_thread::get_id()),
	m_thread(
		[&]
		{
			while (!m_abort_thread)
			{
				if (m_trigger)
				{
					(*m_function)();
					m_trigger = false;
				}
			}
		})
{

}

void persistent_thread::execute(
	std::function<void()>& a_function
)
{
	assert(std::this_thread::get_id() == m_owner_id);
	assert(!m_trigger);

	m_function = &a_function;
	m_trigger = true;

}

void persistent_thread::join(

)
{
	while (m_trigger);
}
