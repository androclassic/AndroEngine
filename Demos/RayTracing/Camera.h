#pragma once
#include "AndroUtils\Utils\Ray.h"
#include "../../AndroUtils/EventManager/EventManager.h"

#include <memory>


class Camera : public andro::EventListener
{
private:
	bool m_mousemove;

public:
	Camera(andro::Vector3& position, andro::Vector3& lookat, float vfov, float aspect_ratio, float focus_dist, float aperture);
	void OnEvent(andro::Event *e);
	void Move(float dx, float dz);
	void UpdateCamera();
	andro::ray getRay(float u, float v) const;

	andro::Vector3 m_top_left_corner;
	andro::Vector3 m_horizontal; 
	andro::Vector3 m_vertical;
	andro::Vector3 m_origin;
	andro::Vector3 m_right;
	andro::Vector3 m_front;
	andro::Vector3 m_up;

	float half_width;
	float half_height;
	float lens_radius;
	float focus_distance;


};