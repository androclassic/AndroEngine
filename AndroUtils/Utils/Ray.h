#pragma once
#include "MMath.h"

namespace andro
{
	class Hitable;


	struct ray
	{
		ray()
			:origin()
			, dir()
			, _time(0.0f)
		{};

		ray(const Vector3& pOrigin, const Vector3& pDir, afloat pTime = 0.0f)
			: origin(pOrigin)
			, dir(pDir)
			, _time(pTime)
		{
			dir.NormalizeInto();
		}

		inline Vector3 get_point_at(afloat t) const
		{
			return origin + (dir * t);
		}
		inline afloat get_time() const
		{
			return _time;
		}

		Vector3 origin;
		Vector3 dir;

		//exit time(optional)
	private:
		afloat _time; 

	};

	struct hit_record
	{
		afloat t;
		Vector2 uv;
		Vector3 normal;
		Vector3 point;
		const Hitable* object;
	};

	class Hitable
	{
		public:
			virtual bool hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const = 0;
			virtual Vector2 getUV(const Vector3& point) const = 0;

	};



}