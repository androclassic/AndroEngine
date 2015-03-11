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

MouseLButtonPressed::MouseLButtonPressed(float x,float y):Event(ID())
{
	m_x=x;
	m_y=y;
}

MouseLButtonUp::MouseLButtonUp(float x,float y):Event(ID())
{
	
	m_x=x;
	m_y=y;
}


float MouseMove::m_lastx=0;
float MouseMove::m_lasty=0;

MouseMove::MouseMove(float x,float y):Event(ID())
{
	
	m_x=x;
	m_y=y;

}
MouseMove::~MouseMove()
{
	
	m_lastx=m_x;
	m_lasty=m_y;

}

