#ifndef _EVENT_H_
#define _EVENT_H_

#include"Types.h"

namespace andro
{

	#ifndef DeclareEventID
	#define DeclareEventID(X)  static u32 ID(){return X;}
	#endif

	class Event
	{
	public:
		u32 GetID()const;
		void Destroy();
	protected:
		Event(u32 _id); 
		virtual ~Event(){}
	   
	private:
		u32 m_id;
	};
}

#endif