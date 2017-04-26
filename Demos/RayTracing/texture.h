#pragma once
#include "../AndroUtils/Utils/Ray.h"
#include "../AndroUtils/Utils/perlin_noise.h"

using namespace andro;

class CTexture
{
	public:
		DEVICE_HOST virtual Vector3 sample(afloat u, afloat v, const Vector3& p) const = 0;
};

class constant_texture : public CTexture
{
	public:
		DEVICE_HOST constant_texture(){}

		DEVICE_HOST constant_texture(const Vector3& color)
			: m_color(color)
		{}

		DEVICE_HOST Vector3 sample(afloat u, afloat v, const Vector3& p) const
		{
			return m_color;
		}

	private:
		Vector3 m_color;
};


class image_texture : public CTexture
{
public:
	DEVICE_HOST image_texture(){}
	DEVICE_HOST image_texture(unsigned char* pixels, unsigned int w, unsigned int h) : data(pixels), m_width(w), m_height(h)
	{}

	DEVICE_HOST Vector3 sample(afloat u, afloat v, const Vector3& p) const;
private:
	unsigned char* data;
	unsigned int m_width, m_height;
};


class checker_texture : public CTexture
{
public:
	DEVICE_HOST checker_texture(){}

	DEVICE_HOST checker_texture(const Vector3& color1, const Vector3& color2, unsigned int size)
		: m_color1(color1)
		, m_color2(color2)
		, m_size(size)
	{}

	DEVICE_HOST Vector3 sample(afloat u, afloat v, const Vector3& p) const;

private:
	Vector3 m_color1;
	Vector3 m_color2;
	unsigned int m_size;
};


class noise_texture : public CTexture
{
public:
	DEVICE_HOST noise_texture(){}
	DEVICE_HOST noise_texture(const Vector3& color)
		: m_color(color)
#ifndef _USE_CUDA
		, noise()
#endif
	{}

	DEVICE_HOST Vector3 sample(afloat u, afloat v, const Vector3& p) const;


private:
	Vector3 m_color;
#ifndef _USE_CUDA
	perlin_noise noise;
#endif

};