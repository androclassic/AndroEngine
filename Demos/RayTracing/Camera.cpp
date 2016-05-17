#include "Camera.h"
#include <windows.h>

using namespace andro;

Camera::Camera(andro::Vector3& position, andro::Vector3& lookat, float vfov, float aspect_ratio)
{
	REGISTER_LISTENER(KeyPressedEvent::ID(), this);
	REGISTER_LISTENER(MouseMove::ID(), this);
	REGISTER_LISTENER(MouseLButtonUp::ID(), this);
	REGISTER_LISTENER(MouseLButtonPressed::ID(), this);


	float theta = DEG2RAD(vfov);
	half_height = tan(theta / 2);
	half_width = aspect_ratio * half_height;

	m_mousemove = false;

	Vector3 absoluteUp(0, 1, 0);

	m_front =   position - lookat;
	m_front.Normalize();
	m_right = absoluteUp.vectorProduct(m_front);
	m_up = m_front.vectorProduct(m_right);
	m_origin = position;
	UpdateCamera();
}


void  Camera::Move(float dx, float dz)
{
	if (dz)
	{
		m_origin = m_origin - (m_front * dz);

	}
	else if (dx)
	{
		m_origin = m_origin - (m_right * dx);
	}

}

void Camera::UpdateCamera()
{
	//update camera
	m_top_left_corner = m_origin - (m_right * half_width) + (m_up *  half_height) - m_front;

	m_horizontal = m_right * half_width * 2;
	m_vertical = m_up * half_height * -2;

}

void Camera::OnEvent(andro::Event *e)
{
	if (e->GetID() == KeyPressedEvent::ID())// 0<- event blocked 
	{
		KeyPressedEvent* event = static_cast<KeyPressedEvent*>(e);
		switch (event->GetKey())
		{


		case 0x41:
		case VK_LEFT:
			Move(0.1, 0);
			break;
		case 0x44:
		case VK_RIGHT:
			Move(-0.1, 0);
			break;
		case 0x57:
		case VK_UP:
			Move(0, 0.1);
			break;
		case 0x53:
		case VK_DOWN:
			Move(0, -0.1);
			break;
		}
		
	}

	else if (e->GetID() == MouseMove::ID() && m_mousemove)
	{
		MouseMove* event = static_cast<MouseMove*>(e);
		float angle_y = ((float)(event->m_x - event->m_lastx) / 5 );
		float angle_x = ((float)(event->m_y - event->m_lasty) / 5);
		angle_x = DEG2RAD(angle_x);
		angle_y = DEG2RAD(angle_y);

		//rotate around world y axis
		Matrix4 rotate_yaw(cos(angle_y), 0, sin(angle_y), 0,
							0, 1, 0, 0,
							-sin(angle_y), 0, cos(angle_y), 0,
							0, 0, 0, 1);

		m_front = m_front *rotate_yaw;
		m_right = m_right *rotate_yaw;

		m_right.Normalize();
		m_front.Normalize();


	}


	else if (e->GetID() == MouseLButtonPressed::ID())
	{
		m_mousemove = true;

	}
	else if (e->GetID() == MouseLButtonUp::ID())
	{
		m_mousemove = false;

	}

	UpdateCamera();

}


andro::ray Camera::getRay(float u, float v) const
{
	return andro::ray(m_origin, m_top_left_corner + (m_horizontal * u) + (m_vertical * v) - m_origin);
}


