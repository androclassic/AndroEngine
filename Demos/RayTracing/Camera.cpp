#include "Camera.h"

andro::ray Camera::getRay(float u, float v) const
{
	return andro::ray(m_origin, m_top_left_corner + (m_horizontal * u) + (m_vertical * v));
}