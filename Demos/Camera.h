#pragma once

#include "../../EventManager/EventManager.h"
#include "../../AndroRenderer/AndroRenderer.h"
#include "../../EventManager/EventManager.h"

#include <memory>
#include "GLFW\glfw3.h"

class Camera : public andro::EventListener
{
private:
	bool m_mousemove;
	std::shared_ptr<andro::Camera> m_camera;

public:
	Camera();
	void OnEvent(andro::Event *e);
	std::shared_ptr<andro::Camera> GetCamera() const { return m_camera; }

};

Camera::Camera()
{
	REGISTER_LISTENER(KeyPressedEvent::ID(),this);
	REGISTER_LISTENER(MouseMove::ID(),this);
	REGISTER_LISTENER(MouseLButtonUp::ID(),this);
	REGISTER_LISTENER(MouseLButtonPressed::ID(),this);

	m_mousemove=false;
	m_camera = std::make_shared<andro::Camera>();
}

void Camera::OnEvent(andro::Event *e)
{
	if(e->GetID() == KeyPressedEvent::ID() )// 0<- event blocked 
	{
		KeyPressedEvent* event = static_cast<KeyPressedEvent*>(e);
		switch(event->GetKey())
		{


			case GLFW_KEY_LEFT:
				m_camera->Move(-1,0);
				break;
			case GLFW_KEY_RIGHT:
				m_camera->Move(1, 0);
				break;
			case GLFW_KEY_UP:
				m_camera->Move(0, -1);
				break;
			case GLFW_KEY_DOWN:
				m_camera->Move(0, 1);
				break;
			}
	}

	else if(e->GetID() == MouseMove::ID() && m_mousemove ) 
	{
		MouseMove* event = static_cast<MouseMove*>(e);
		m_camera->AddYaw((float)(event->m_x-event->m_lastx)/5);
		m_camera->AddPitch((float)(event->m_y-event->m_lasty)/5);

	}


	else if(e->GetID() == MouseLButtonPressed::ID() ) 
	{
		m_mousemove=true;

	}
	else if(e->GetID() == MouseLButtonUp::ID() ) 
	{
		m_mousemove=false;

	}
	
}
