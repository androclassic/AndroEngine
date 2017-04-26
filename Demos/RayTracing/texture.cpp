#pragma once
#include "texture.h"
#include "AndroUtils\Utils\MMath.h"


DEVICE_HOST Vector3 checker_texture::sample(afloat u, afloat v, const Vector3& p) const
{
	afloat sines = sinf((m_size * p.x))*sinf((m_size * p.y))*sinf((m_size * p.z));

	if (sines > 0)
		return m_color1;
	else
		return m_color2;
}

DEVICE_HOST Vector3 image_texture::sample(afloat u, afloat v, const Vector3& p) const
{
	int i = u * m_width;
	int j = (1 - v) * m_height - 0.0001f;

	if (i < 0)	i = 0;
	if (j < 0)	j = 0;
	if (i > (m_width - 1)) i = m_width -1 ;
	if (j >(m_height - 1)) j = m_height - 1;

	afloat r = int(data[3 * i + 3 * m_width * j]) / 255.0f;
	afloat g = int(data[3 * i + 3 * m_width * j + 1]) / 255.0f;
	afloat b = int(data[3 * i + 3 * m_width * j + 2]) / 255.0f;

	return Vector3(r, g, b);
}

DEVICE_HOST Vector3 noise_texture::sample(afloat u, afloat v, const Vector3& p) const
{
#ifndef _USE_CUDA
	return m_color* noise.get_value(p);
#else
	afloat sines = sinf((10 * p.x))*sinf((10 * p.y))*sinf((10 * p.z));

	if (sines > 0)
		return m_color;
	else
		return Vector3(0, 0, 0);
#endif
}
