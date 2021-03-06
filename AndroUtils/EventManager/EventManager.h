#ifndef _EV_MAN_H_
#define _EV_MAN_H_

#include<vector>
#include<map>
#include"Singleton.h"

#ifndef ANDRO_EVENT_MANAGER
#define ANDRO_EVENT_MANAGER 
#endif

#include "Event.h"
#include "WindowsInputEvents.h"

using namespace std;


namespace andro
{
	#ifndef PostEvent
	#define PostEvent(X) andro::EventManager::GetInstance()->AddEvent(X)
	#endif

	#ifndef REGISTER_LISTENER
	#define REGISTER_LISTENER(ID,LISTENER)  andro::EventManager::GetInstance()->RegisterEventListener(ID,LISTENER)
	#endif

#ifndef UNREGISTER_LISTENER
#define UNREGISTER_LISTENER(ID,LISTENER)  andro::EventManager::GetInstance()->UnregisterEventListener(ID,LISTENER)
#endif

	


	class EventListener
	{
		public:
			virtual void OnEvent(Event* e) = 0;
	};

	class EventManager
	{
	  SINGLETON(EventManager);
	  void Update(f32 dt);
	public:
	void AddEvent(Event* rhs);
	void RegisterEventListener(u32 EventID,EventListener* newListner);
	void UnregisterEventListener(u32 EventID, EventListener* listner);
	private:
		std::map<u32, std::vector<EventListener*> > m_listenersMap;
		std::vector<Event*>    m_newEvents;
		std::vector<Event*>    m_oldEvents;
	};
}
#endif