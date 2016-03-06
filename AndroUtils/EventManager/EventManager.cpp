//Georgescu Andrei 2011//

#include "EventManager.h"

namespace andro
{

	EventManager::EventManager(void)
	{
	}
	EventManager::~EventManager(void)
	{
	}
	void EventManager::AddEvent(Event* rhs)
	{
		m_oldEvents.push_back(rhs);
	}

	void EventManager::RegisterEventListener(u32 id,EventListener* listener)
	{
		for(vector<listeners>::iterator  it=m_listenersList.begin();it!=m_listenersList.end();++it)
		{
			if(it->m_id==id)
			{
				it->m_listeners.push_back(listener);
				return;
			}
		}

		listeners newList;
		newList.m_id=id;
		newList.m_listeners.push_back(listener);
		m_listenersList.push_back(newList);
	}
	void EventManager::Update(f32 dt)
	{
		
		for(vector<Event*>::iterator _event = m_oldEvents.begin();_event != m_oldEvents.end();_event++)
		{
			for(vector<listeners>::iterator _listenerList = m_listenersList.begin();_listenerList != m_listenersList.end();_listenerList++)
			{
				if(_listenerList->m_id==(*_event)->GetID())
				{
					for(vector<EventListener*>::iterator _listener = _listenerList->m_listeners.begin(); _listener != _listenerList->m_listeners.end();_listener++)
						(*_listener)->OnEvent((*_event));
				}
			}
			(*_event)->Destroy();
		}
		m_oldEvents.clear();

	}
}