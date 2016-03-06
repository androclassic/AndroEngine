#pragma once

#include "Event.h"

class KeyPressedEvent : public andro::Event
{
	public:
		KeyPressedEvent(u32 key);

		DeclareEventID('KEYP');

		u32		GetKey();

	private:
		u32		m_key;
};


class KeyUpEvent : public andro::Event
{
	public:
		KeyUpEvent(u32 key);

		DeclareEventID('KEYU');

		u32		GetKey();

	private:
		u32		m_key;
};


class MouseLButtonPressed : public andro::Event
{
	public:
		MouseLButtonPressed(float x,float y);

		DeclareEventID('LB_P');
    private:
		float m_x;
		float m_y;
		
};


class MouseLButtonUp: public andro::Event
{
	public:
		MouseLButtonUp(float x,float y);

		DeclareEventID('LB_U');
    private:
		float m_x;
		float m_y;

		
};



class MouseMove: public andro::Event
{
	public:
		MouseMove(float x,float y);
virtual	~MouseMove();

		DeclareEventID('MM');

		float m_x;
		float m_y;
		static float m_lastx;
		static float m_lasty;

		
};
