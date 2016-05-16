#include "Material.h"

bool Lambertian::scatter(const ray & ray_in, const hit_record & rec, Vector3 & attenuation, ray & scattered) const
{
	andro::Vector3 dir = (rec.normal + andro::random_in_unit_sphere());
	scattered = andro::ray(rec.point, dir);
	attenuation = albedo;
	return true;
}


andro::Vector3 Reflect(const andro::Vector3& vector, const andro::Vector3& normal)
{
	return vector - (normal * (vector * normal)* 2.0f);
}


bool Metal::scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const
{
	andro::Vector3 ray_dir = ray_in.dir;
	ray_dir.Normalize();
	andro::Vector3 reflect = Reflect(ray_dir, rec.normal) + (random_in_unit_sphere() *  roughness);
	scattered = andro::ray(rec.point, reflect);
	attenuation = albedo;

	return ((scattered.dir *  rec.normal) > 0.0f);

}
