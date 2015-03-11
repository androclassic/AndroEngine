//Georgescu Andrei 2011//

#include "Event.h"
namespace andro
{

	Event::Event(u32 _id)
	{
		m_id=_id;
	}

	u32 Event::GetID() const
	{
		return m_id;
	}
	void Event::Destroy()
	{
	  delete this;
	}
}