#pragma once
#include "MMath.h"

namespace andro
{
	class Hitable;


	struct ray
	{
		ray() {};
		ray(const Vector3& o, const Vector3& d) : origin(o), dir(d) 
		{
			dir.NormalizeInto();
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
		const Hitable* object;
	};

	class Hitable
	{
		public:
			virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
	};



}