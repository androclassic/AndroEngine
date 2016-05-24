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
		Sphere(Vector3 c, float r) :center(c), radius(r) {}
		bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

		float radius;
		Vector3 center;

	};

	struct BoundingBox : public Hitable
	{
		BoundingBox();
		inline void SetExtents(Vector3& pMin, Vector3&  pMax) { min = pMin; max = pMax; }
		Vector3 GetHalfSize() const;
		Vector3 GetCenter() const;

		inline bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

		Vector3		min;
		Vector3		max;
	};


	//intersections
	bool  Box_Plane_Overlap(const BoundingBox& box, Vector3 planeNormal, float distance);
	bool  TriangleBoxOverlap(const BoundingBox& box, Triangle& triangle);
	bool  Box_Sphere_Overlap(const BoundingBox& box, Sphere& sphere);
}


