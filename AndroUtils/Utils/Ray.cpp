#include "Ray.h"

namespace andro
{
	bool Sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
	{
		// p(t) = (origin + t * dir)) - center)
		//dot( ( p(t), p(t)) = R* R

		Vector3 oc = r.origin - center;
		float a = r.dir * r.dir;
		float b = (oc * r.dir);
		float c = (oc * oc) - (radius * radius);

		float discriminant = b*b - a*c;

		if (discriminant < 0)
			return false;

		float temp = (-b - sqrtf(discriminant) / a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.point = r.get_point_at(temp);
			rec.normal = (rec.point - center) * (1.0f/radius);
			rec.object = this;
			return true;
		}

		temp = (-b + sqrtf(discriminant) / a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.point = r.get_point_at(temp);
			rec.normal = (rec.point - center) * (1.0f / radius);
			rec.object = this;
			return true;
		}

		return false;
	}

}
