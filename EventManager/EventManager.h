//Georgescu Andrei 2011//

#ifndef _EV_MAN_H_
#define _EV_MAN_H_
#include"EventListener.h"
#include"Singleton.h"

#ifndef ANDRO_EVENT_MANAGER
#define ANDRO_EVENT_MANAGER 
#endif

#ifdef _WIN32
#include "WindowsInputEvents.h"
#endif

using namespace std;


namespace andro
{
	#ifndef PostEvent
	#define PostEvent(X) andro::EventManager::GetInstance()->AddEvent(X)
	#endif

	#ifndef REGISTER_LISTENER
	#define REGISTER_LISTENER(ID,LISTENER)  andro::EventManager::GetInstance()->RegisterEventListener(ID,LISTENER)
	#endif


	class EventManager
	{
	  SINGLETON(EventManager);
	  void Update(f32 dt);
	public:
	void AddEvent(Event* rhs);
	void RegisterEventListener(u32 EventID,EventListener* newListner);
	private:
		struct listeners
		{
			u32 m_id;
			vector<EventListener*> m_listeners;
		};
		vector<listeners> m_listenersList;
		vector<Event*>    m_newEvents;
		vector<Event*>    m_oldEvents;
	};
}
#endif