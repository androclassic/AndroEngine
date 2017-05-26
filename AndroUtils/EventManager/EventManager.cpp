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
		m_listenersMap[id].push_back(listener);
	}

	void EventManager::UnregisterEventListener(u32 id, EventListener* listner)
	{
		auto it = std::find(m_listenersMap[id].begin(), m_listenersMap[id].end(), listner);
		if (it != m_listenersMap[id].end())
			m_listenersMap[id].erase(it);
	}


	void EventManager::Update(f32 dt)
	{
		
		for(vector<Event*>::iterator _event = m_oldEvents.begin();_event != m_oldEvents.end();_event++)
		{
			u32 event_id = (*_event)->GetID();
			std::vector<EventListener*>& listeners = m_listenersMap[event_id];

			for( auto listener : listeners)
				listener->OnEvent((*_event));

			(*_event)->Destroy();
		}
		m_oldEvents.clear();

	}
}