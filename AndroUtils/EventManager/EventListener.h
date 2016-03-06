//Georgescu Andrei 2011//

#ifndef _EVENT_LISTENER_H_
#define _EVENT_LISTENER_H_
#include<vector>
#include"Event.h"
using namespace std;

namespace andro
{

	class EventListener
	{
	public:   
		inline void AddEvent(Event* rhs){m_events.push_back(rhs);};
		virtual void OnEvent(Event* e)=0;
	protected:
		void RemoveEvents();
	private:
		vector<Event*> m_events;

	};
}
#endif