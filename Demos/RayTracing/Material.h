#pragma once
#include "texture.h"
#include "AndroUtils/Utils/Ray.h"

using namespace andro;

class material
{
	public:
		DEVICE virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const = 0;
		DEVICE_HOST virtual Vector3 emitted(afloat u, afloat v, const Vector3& p) const
		{
			return Vector3();
		}
		DEVICE_HOST virtual void Destroy() {};
};

class Lambertian : public material
{
	public:
		DEVICE_HOST Lambertian() {}
		DEVICE_HOST Lambertian(CTexture* pTexture) : m_texture(pTexture) {}
		DEVICE_HOST virtual void Destroy()  { if (m_texture){ delete m_texture; m_texture = NULL; } }
		DEVICE bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
		CTexture* m_texture;
};


class Isotropic : public material
{
public:
	DEVICE_HOST Isotropic() {}
	DEVICE_HOST Isotropic(CTexture* pTexture) : m_texture(pTexture) {}
	DEVICE_HOST virtual void Destroy()  { if (m_texture){ delete m_texture; m_texture = NULL; } }
	DEVICE bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
	CTexture* m_texture;
};

class Metal : public material
{
public:
	DEVICE_HOST Metal() { }
	DEVICE_HOST Metal(const andro::Vector3& color, afloat pRoughness) : albedo(color), roughness(pRoughness){}
	DEVICE bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
	Vector3 albedo;
	afloat	roughness;
};

class Dielectric : public material
{
public:
	DEVICE_HOST Dielectric(){}
	DEVICE_HOST Dielectric(afloat refractive_idx) : refractive_index(refractive_idx) {}
	DEVICE bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;

	afloat refractive_index;

};


class diffuse_light : public material
{
public:
	DEVICE_HOST diffuse_light() {}
	DEVICE_HOST diffuse_light(CTexture* pTexture) : m_texture(pTexture) {}
	DEVICE_HOST virtual void Destroy() { if (m_texture){ delete m_texture; m_texture = NULL; } }
	DEVICE	bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const { return false; }
	DEVICE_HOST Vector3 emitted(afloat u, afloat v, const Vector3& p) const
	{
		return m_texture->sample(u, v, p);
	}

	CTexture* m_texture;
};
