#pragma once
#include "Ray.h"

namespace andro
{
	struct Triangle //TODO hitable
	{
		Vector3 P1, P2, P3;
	};


	struct Sphere : public Hitable
	{
		Sphere() {}
		Sphere(Vector3 c, afloat r) :center(c), radius(r) {}
		bool hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const;
		Vector2 getUV(const Vector3& point) const;

		afloat radius;
		Vector3 center;

	};

	struct BoundingBox : public Hitable
	{
		BoundingBox();
		BoundingBox(const Vector3& pCenter, const Vector3& pHalfSize) { min = pCenter - pHalfSize; max = pCenter + pHalfSize; }
		inline void SetExtents(Vector3& pMin, Vector3&  pMax) { min = pMin; max = pMax; }
		Vector3 GetHalfSize() const;
		Vector3 GetCenter() const;

		inline bool hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const;
		inline Vector2 getUV(const Vector3& point) const
		{
			return Vector2(); //todo
		}

		Vector3		min;
		Vector3		max;
	};


	struct xy_rect : public Hitable
	{
		xy_rect(){}
		xy_rect(afloat _x0, afloat _x1, afloat _y0, afloat _y1, afloat _k, afloat normal = 1.0)
			: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), n(normal) {}

		bool hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const;
		Vector2 getUV(const Vector3& point) const;

		afloat x0, x1, y0, y1, k, n;
	};

	struct xz_rect : public Hitable
	{
		xz_rect(){}
		xz_rect(afloat _x0, afloat _x1, afloat _z0, afloat _z1, afloat _k, afloat normal = 1.0)
			: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), n(normal) {}

		bool hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const;
		Vector2 getUV(const Vector3& point) const;

		afloat x0, x1, z0, z1, k, n;
	};

	struct yz_rect : public Hitable
	{
		yz_rect(){}
		yz_rect(afloat _y0, afloat _y1, afloat _z0, afloat _z1, afloat _k, afloat normal = 1.0)
			: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), n(normal) {}

		bool hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const;
		Vector2 getUV(const Vector3& point) const;

		afloat y0, y1, z0, z1, k, n;
	};



	//intersections
	bool  Box_Plane_Overlap(const BoundingBox& box, Vector3 planeNormal, afloat distance);
	bool  TriangleBoxOverlap(const BoundingBox& box, Triangle& triangle);
	bool  Box_Sphere_Overlap(const BoundingBox& box, Sphere& sphere);
}


