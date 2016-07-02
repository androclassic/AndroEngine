#include "Material.h"

bool Lambertian::scatter(const ray & ray_in, const hit_record & rec, Vector3 & attenuation, ray & scattered) const
{
	Vector3 dir = (rec.normal + random_in_unit_sphere());
	scattered = ray(rec.point, dir);

	attenuation = m_texture->sample(rec.uv.x, rec.uv.y, rec.point);
	return true;
}


Vector3 Reflect(const Vector3& vector, const Vector3& normal)
{
	return vector - (normal * (vector * normal)* 2.0f);
}


//Snell's law
bool Refract(const Vector3& v, const Vector3& n, afloat refractive_idx_ratio, Vector3& refracted)
{
	Vector3 unit_v = v.Normalise();

	afloat dt = unit_v * n;
	afloat discriminant = 1.0 - refractive_idx_ratio*refractive_idx_ratio * (1 - dt*dt);
	if (discriminant > 0)
	{
		refracted = (unit_v - (n*dt)) * refractive_idx_ratio  - n * sqrtf(discriminant);
		return true;
	}

	return false;
}


afloat Schlick(afloat cosine, afloat ref_idx)
{
	afloat r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 *r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}


bool Metal::scatter(const ray& ray_in, const hit_record& rec, Vector3& attenuation, ray& scattered) const
{
	Vector3 ray_dir = ray_in.dir.Normalise();
	Vector3 reflect = Reflect(ray_dir, rec.normal) + (random_in_unit_sphere() *  roughness);
	scattered = ray(rec.point, reflect);
	attenuation = albedo;

	return ((scattered.dir *  rec.normal) > 0.0f);

}
bool Dielectric::scatter(const ray & ray_in, const hit_record & rec, Vector3 & attenuation, ray & scattered) const
{
	Vector3 outward_normal;
	Vector3 refracted;
	afloat refractive_index_ratio, reflect_prob, cosine;

	Vector3 reflected = Reflect(ray_in.dir, rec.normal);
	attenuation = Vector3(1.0f, 1.0f, 1.0f);

	if ((ray_in.dir * rec.normal) > 0)
	{
		outward_normal = (rec.normal * -1);
		refractive_index_ratio = refractive_index;
		cosine =  (ray_in.dir * rec.normal) / ray_in.dir.Lenght();
		cosine = 1 - refractive_index * refractive_index* (1 - cosine *cosine);
	}
	else
	{
		outward_normal = (rec.normal);
		refractive_index_ratio = 1.0f/refractive_index;
		cosine = -1.0f * refractive_index *  (ray_in.dir * rec.normal) / ray_in.dir.Lenght() ;
	}

	outward_normal.NormalizeInto();

	if (Refract(ray_in.dir, outward_normal, refractive_index_ratio, refracted))
	{
		reflect_prob = Schlick(cosine, refractive_index);
	}
	else
	{
		reflect_prob = 2.0f;
	}
	if (andro::random_float(1.0f) < reflect_prob)
	{
		scattered = ray(rec.point, reflected);
	}
	else
		scattered = ray(rec.point, refracted);

	return true;
}
