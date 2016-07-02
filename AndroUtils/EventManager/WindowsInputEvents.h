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
		MouseLButtonPressed(afloat x,afloat y);

		DeclareEventID('LB_P');
    private:
		afloat m_x;
		afloat m_y;
		
};


class MouseLButtonUp: public andro::Event
{
	public:
		MouseLButtonUp(afloat x,afloat y);

		DeclareEventID('LB_U');
    private:
		afloat m_x;
		afloat m_y;

		
};



class MouseMove: public andro::Event
{
	public:
		MouseMove(afloat x,afloat y);
virtual	~MouseMove();

		DeclareEventID('MM');

		afloat m_x;
		afloat m_y;
		static afloat m_lastx;
		static afloat m_lasty;

		
};
