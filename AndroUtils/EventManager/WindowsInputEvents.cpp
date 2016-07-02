#include "WindowsInputEvents.h"

KeyPressedEvent::KeyPressedEvent(u32 key) : Event(ID())
{
	m_key = key;
}

u32 KeyPressedEvent::GetKey()
{
	return m_key;
}



KeyUpEvent::KeyUpEvent(u32 key) : Event(ID())
{
	m_key = key;
}

u32 KeyUpEvent::GetKey()
{
	return m_key;
}

MouseLButtonPressed::MouseLButtonPressed(afloat x,afloat y):Event(ID())
{
	m_x=x;
	m_y=y;
}

MouseLButtonUp::MouseLButtonUp(afloat x,afloat y):Event(ID())
{
	
	m_x=x;
	m_y=y;
}


afloat MouseMove::m_lastx=0;
afloat MouseMove::m_lasty=0;

MouseMove::MouseMove(afloat x,afloat y):Event(ID())
{
	
	m_x=x;
	m_y=y;

}
MouseMove::~MouseMove()
{
	
	m_lastx=m_x;
	m_lasty=m_y;

}

