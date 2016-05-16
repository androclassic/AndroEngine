#pragma once
#include "AndroUtils\Utils\Ray.h"

class Camera
{
	public:
		Camera(float ratio)
		{
			m_top_left_corner = andro::Vector3(-ratio, 1.0f, -1.0f);
			m_horizontal = andro::Vector3(ratio * 2, 0.0f, 0.0f); // -1 - 1
			m_vertical = andro::Vector3(0.0f, -2.0f, 0.0f); // -1 - 1

		}

		andro::ray getRay(float u, float v) const;

		andro::Vector3 m_top_left_corner;
		andro::Vector3 m_horizontal; 
		andro::Vector3 m_vertical;
		andro::Vector3 m_origin;

};