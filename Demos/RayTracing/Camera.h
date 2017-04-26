#pragma once
#include "AndroUtils/Utils/Ray.h"
#include "AndroUtils/Utils/AndroUtils.h"
#include "AndroUtils/EventManager/EventManager.h"

#include <memory>

DEVICE andro::Vector3 random_in_unit_disk();
class Camera : public andro::EventListener
{
private:
	bool m_mousemove;

public:
	Camera(andro::Vector3& position, andro::Vector3& lookat, afloat vfov, afloat aspect_ratio, afloat focus_dist, afloat aperture, afloat t0, afloat t1);
	void OnEvent(andro::Event *e);
	void Move(afloat dx, afloat dz);
	void UpdateCamera();


	DEVICE andro::ray Camera::getRay(afloat u, afloat v) const
	{
		andro::Vector3 rd = random_in_unit_disk() * lens_radius;
		andro::Vector3 offset = m_right * rd.x + m_up*rd.y;
		andro::Vector3 origin = m_origin;// +offset;
		return andro::ray(origin, m_top_left_corner + (m_horizontal * u) + (m_vertical * v) - origin, andro::GetTimeMS());
	}

	andro::Vector3 m_top_left_corner;
	andro::Vector3 m_horizontal; 
	andro::Vector3 m_vertical;
	andro::Vector3 m_origin;
	andro::Vector3 m_right;
	andro::Vector3 m_front;
	andro::Vector3 m_up;

	afloat half_width;
	afloat half_height;
	afloat lens_radius;
	afloat focus_distance;
	afloat time0, time1; // sutter open-close times


};