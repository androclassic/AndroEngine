#pragma once
#include "AndroUtils\Utils\Ray.h"
#include "AndroUtils\Utils\perlin_noise.h"

using namespace andro;

class texture
{
	public:
		virtual Vector3 sample(afloat u, afloat v, const Vector3& p) const = 0;
};

class constant_texture : public texture
{
	public:
		constant_texture(){}

		constant_texture(const Vector3& color)
			: m_color(color)
		{}

		Vector3 sample(afloat u, afloat v, const Vector3& p) const
		{
			return m_color;
		}

	private:
		Vector3 m_color;
};


class image_texture : public texture
{
public:
	image_texture(){}
	image_texture(unsigned char* pixels, unsigned int w, unsigned int h) : data(pixels), m_width(w), m_height(h)
	{}

	Vector3 sample(afloat u, afloat v, const Vector3& p) const;
private:
	unsigned char* data;
	unsigned int m_width, m_height;
};


class checker_texture : public texture
{
public:
	checker_texture(){}

	checker_texture(const Vector3& color1, const Vector3& color2, unsigned int size)
		: m_color1(color1)
		, m_color2(color2)
		, m_size(size)
	{}

	Vector3 sample(afloat u, afloat v, const Vector3& p) const;

private:
	Vector3 m_color1;
	Vector3 m_color2;
	unsigned int m_size;
};


class noise_texture : public texture
{
public:
	noise_texture(){}
	noise_texture(const Vector3& color)
		: m_color(color)
		, noise()
	{}

	Vector3 sample(afloat u, afloat v, const Vector3& p) const
	{
		return m_color * noise.get_value(p);
	}

private:
	Vector3 m_color;
	perlin_noise noise;

};