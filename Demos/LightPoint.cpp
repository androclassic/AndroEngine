#include "LightPoint.h"

LightPoint::~LightPoint()
{

	if(m_LightObject != NULL)
	{
		delete m_LightObject;
		m_LightObject = NULL;
	}

}
LightPoint::LightPoint(andro::Vector3 pos, andro::Vector3 lookAt)
{
	REGISTER_LISTENER(KeyPressedEvent::ID(),this);
	m_LightObject = NULL;
	//m_LightObject = new Sphere(1, AndroToForce(pos),false, force::Quaternion(), 1);
	m_light = true;
	m_shadow = true;

	m_position = pos;
	m_lookAt = lookAt;
	andro::Engine::GetInstance()->SetLightPositionDir(pos, lookAt);

}


void LightPoint::SetPositionAndLook(andro::Vector3 pos, andro::Vector3 lookAt)
{
	m_position = pos;
	m_lookAt = lookAt;
	andro::Engine::GetInstance()->SetLightPositionDir(pos, lookAt);
	//m_LightObject->GetVisualObject()->SetPosition(pos);
}
void LightPoint::EnableShadow(bool state)
{
	m_shadow = state;
	andro::Engine::GetInstance()->EnableShadow(state);
}
void LightPoint::EnableLight(bool state)
{
	m_light	= state;
}

void LightPoint::OnEvent(andro::Event *e)
{
	if(e->GetID() == KeyPressedEvent::ID() )// 0<- event blocked 
	{
		KeyPressedEvent* event = static_cast<KeyPressedEvent*>(e);
		switch(event->GetKey())
		{
			case 'C':
				EnableShadow(!m_shadow); //shadow trigger
				break;
			case 'V':
				//to do
				break;
		}
	}
}



void LightPoint::Update(float dt)
{
	//double alpha = dt/10;
	//andro::Vector3 v;
	//v.x = m_position.x * cos(alpha) - m_position.z * sin(alpha);
	//v.z = m_position.x * sin(alpha) + m_position.z * cos(alpha);
	//v.y = m_position.y;
	//SetPositionAndLook(v,m_lookAt);

}
