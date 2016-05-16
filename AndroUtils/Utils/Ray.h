#pragma once
#include "MMath.h"

namespace andro
{
	struct ray
	{
		ray(const Vector3& o, const Vector3& d) : origin(o), dir(d) 
		{
			dir.Normalize(); 
		}

		Vector3 get_point_at(float t) const
		{
			return origin + (dir * t);
		}

		Vector3 origin;
		Vector3 dir;
	};


	struct hit_record
	{
		float t;
		Vector3 normal;
		Vector3 point;
	};

	class Hitable
	{
		public:
			virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	};


	struct Sphere : public Hitable
	{
			Sphere() {}
			Sphere(Vector3 c, float r) :center(c), radius(r) {}
			bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

			float radius;
			Vector3 center;



	};

}