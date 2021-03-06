#include "Camera.h"
#include <windows.h>

using namespace andro;

Camera::Camera(andro::Vector3& position, andro::Vector3& lookat, afloat vfov, afloat aspect_ratio, afloat focus_dist, afloat aperture, afloat t0, afloat t1)
	: m_origin(position)
	, focus_distance(focus_dist)
	, lens_radius(aperture / 2.0f)
	, time0(t0)
	, time1(t1)
{
	REGISTER_LISTENER(KeyPressedEvent::ID(), this);
	REGISTER_LISTENER(MouseMove::ID(), this);
	REGISTER_LISTENER(MouseLButtonUp::ID(), this);
	REGISTER_LISTENER(MouseLButtonPressed::ID(), this);


	afloat theta = DEG2RAD(vfov);
	half_height = tan(theta / 2);
	half_width = aspect_ratio * half_height;

	m_mousemove = false;

	Vector3 absoluteUp(0, 1, 0);

	m_front =  (position - lookat).Normalise();
	m_right = absoluteUp.vectorProduct(m_front);
	m_up = m_front.vectorProduct(m_right);
	UpdateCamera();
}

Camera::~Camera()
{
	UNREGISTER_LISTENER(KeyPressedEvent::ID(), this);
	UNREGISTER_LISTENER(MouseMove::ID(), this);
	UNREGISTER_LISTENER(MouseLButtonUp::ID(), this);
	UNREGISTER_LISTENER(MouseLButtonPressed::ID(), this);
}

void  Camera::Move(afloat dx, afloat dz)
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
	m_top_left_corner = m_origin - ((m_right * half_width) - (m_up *  half_height) + m_front) * focus_distance;

	m_horizontal = m_right * half_width * 2 *  focus_distance;
	m_vertical = m_up * half_height * -2 * focus_distance;

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
		afloat angle_y = ((afloat)(event->m_x - event->m_lastx) / 5 );
		afloat angle_x = ((afloat)(event->m_y - event->m_lasty) / 5);
		angle_x = DEG2RAD(angle_x);
		angle_y = DEG2RAD(angle_y);

		//rotate around world y axis
		Matrix4 rotate_yaw(cos(angle_y), 0, sin(angle_y), 0,
							0, 1, 0, 0,
							-sin(angle_y), 0, cos(angle_y), 0,
							0, 0, 0, 1);

		m_front = (m_front *rotate_yaw).Normalise();
		m_right = (m_right *rotate_yaw).Normalise();



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



DEVICE andro::Vector3 random_in_unit_disk()
{
#ifdef _USE_CUDA
	return andro::Vector3(1, 1, 1);
#endif
	andro::Vector3 p;
	do
	{
		p =  andro::Vector3(random_float(2) - 1, random_float(2) - 1, 0);
	} while ((p*p) >= 1);

	return p;
}



