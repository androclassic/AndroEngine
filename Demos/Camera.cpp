#include "Camera.h"

Camera::Camera()
{
	REGISTER_LISTENER(KeyPressedEvent::ID(), this);
	REGISTER_LISTENER(MouseMove::ID(), this);
	REGISTER_LISTENER(MouseLButtonUp::ID(), this);
	REGISTER_LISTENER(MouseLButtonPressed::ID(), this);

	m_mousemove = false;
	m_camera = std::make_shared<TakeTwo::CameraNode>();
}


void  Camera::Move(afloat dx, afloat dz)
{
	glm::vec3 pos = m_camera->GetPosition();
	if (dz)
	{
		pos = pos - (m_camera->GetFrontDir() * (float)dz);

	}
	else if (dx)
	{
		pos = pos - (m_camera->GetRightDir() * (float)dx);
	}

	m_camera->SetPosition(pos);

	glm::mat4 view = m_camera->GetViewMatrix();
}

void Camera::OnEvent(andro::Event *e)
{
	if (e->GetID() == KeyPressedEvent::ID())// 0<- event blocked 
	{
		KeyPressedEvent* event = static_cast<KeyPressedEvent*>(e);
		switch (event->GetKey())
		{


		case GLFW_KEY_A:
		case GLFW_KEY_LEFT:
			Move(-1, 0);
			break;
		case GLFW_KEY_D:
		case GLFW_KEY_RIGHT:
			Move(1, 0);
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_UP:
			Move(0, -1);
			break;
		case GLFW_KEY_S:
		case GLFW_KEY_DOWN:
			Move(0, 1);
			break;
		}
	}

	else if (e->GetID() == MouseMove::ID() && m_mousemove)
	{
		MouseMove* event = static_cast<MouseMove*>(e);
		//rotate around world y axis
		glm::quat rotation = glm::angleAxis((float)(event->m_x - event->m_lastx) / 5, glm::vec3(0, 1, 0));
		m_camera->SetRotation(rotation * m_camera->GetRotation());

		//rotate around camera x axis
		m_camera->Rotate((float)(event->m_y - event->m_lasty) / 5, glm::vec3(1, 0, 0));

	}


	else if (e->GetID() == MouseLButtonPressed::ID())
	{
		m_mousemove = true;

	}
	else if (e->GetID() == MouseLButtonUp::ID())
	{
		m_mousemove = false;

	}
}