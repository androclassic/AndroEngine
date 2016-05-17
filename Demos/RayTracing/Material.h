#pragma once
#include "AndroUtils\Utils\Ray.h"

using namespace andro;

class material
{
	public:
		virtual bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const = 0;
};

class Lambertian : public material
{
	public:
		Lambertian() {}
		Lambertian(const andro::Vector3& color) : albedo(color) {}
		bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
		Vector3 albedo;
};

class Metal : public material
{
public:
	Metal() { }
	Metal(const andro::Vector3& color, float pRoughness ) : albedo(color), roughness(pRoughness){}
	bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;
	Vector3 albedo;
	float	roughness;
};

class Dielectric : public material
{
public:
	Dielectric(){}
	Dielectric(float refractive_idx) : refractive_index(refractive_idx) {}
	bool scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const;

	float refractive_index;

};