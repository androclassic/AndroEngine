#pragma once
#include "texture.h"
#include "AndroUtils/Utils/Ray.h"

using namespace andro;

class material
{
	public:
		virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const = 0;
		virtual Vector3 emitted(afloat u, afloat v, const Vector3& p) const
		{
			return Vector3();
		}
		virtual void Destroy() {};
};

class Lambertian : public material
{
	public:
		Lambertian() {}
		Lambertian(texture* pTexture) : m_texture(pTexture) {}
		virtual void Destroy()  { if (m_texture){ delete m_texture; m_texture = NULL; } }
		bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
		texture* m_texture;
};

class Metal : public material
{
public:
	Metal() { }
	Metal(const andro::Vector3& color, afloat pRoughness ) : albedo(color), roughness(pRoughness){}
	bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
	Vector3 albedo;
	afloat	roughness;
};

class Dielectric : public material
{
public:
	Dielectric(){}
	Dielectric(afloat refractive_idx) : refractive_index(refractive_idx) {}
	bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;

	afloat refractive_index;

};


class diffuse_light : public material
{
public:
	diffuse_light() {}
	diffuse_light(texture* pTexture) : m_texture(pTexture) {}
	virtual void Destroy() { if (m_texture){ delete m_texture; m_texture = NULL; } }
	bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const { return false; }
	Vector3 emitted(afloat u, afloat v, const Vector3& p) const
	{
		return m_texture->sample(u, v, p);
	}

	texture* m_texture;
};
