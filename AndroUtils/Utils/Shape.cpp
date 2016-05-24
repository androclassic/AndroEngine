#include "Shapes.h"

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
			rec.normal = (rec.point - center) * (1.0f / radius);
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




	BoundingBox::BoundingBox() : min(99999.0f, 99999.0f, 99999.0f), max(-99999.0f, -99999.0f, -99999.0f)
	{

	}

	Vector3 BoundingBox::GetHalfSize() const
	{
		Vector3 halfSize = (min + max)*0.5f - min;
		return halfSize;
	}

	Vector3 BoundingBox::GetCenter() const
	{
		return (min + max)*0.5f;
	}


	BoundingBox GetMinimumBoundingBox(Vector3* points, unsigned int num)
	{
		BoundingBox bbx;
		for (unsigned int i = 0; i < num; i++)
		{
			bbx.min.x = std::min(points[i].x, bbx.min.x);
			bbx.min.y = std::min(points[i].y, bbx.min.y);
			bbx.min.z = std::min(points[i].z, bbx.min.z);

			bbx.max.x = std::max(points[i].x, bbx.max.x);
			bbx.max.y = std::max(points[i].y, bbx.max.y);
			bbx.max.z = std::max(points[i].z, bbx.max.z);

		}
		return bbx;
	}

	inline bool BoundingBox::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
	{
		for (int a = 0; a < 3; a++)
		{
			float t0 = fminf((min.v[a] - r.origin.v[a]) / r.dir.v[a],
				(max.v[a] - r.origin.v[a]) / r.dir.v[a]);

			float t1 = fmaxf((min.v[a] - r.origin.v[a]) / r.dir.v[a],
				(max.v[a] - r.origin.v[a]) / r.dir.v[a]);

			t_min = fmaxf(t0, t_min);
			t_max = fminf(t1, t_max);
			if (t_max <= t_min)
				return false;

		}

		return true;
	}



	//------------------------------------------------------------------
	//intersections
	bool  Box_Plane_Overlap(const BoundingBox& box, Vector3 planeNormal, float distance)
	{
		BoundingBox center_box;
		Vector3 halfsize = box.GetHalfSize();

		for (int q = 1; q < 2; q++)
		{
			if (planeNormal.v[q] > 0.0f)
			{
				center_box.min.v[q] = -halfsize.v[q];
				center_box.max.v[q] = halfsize.v[q];
			}
			else
			{
				center_box.min.v[q] = halfsize.v[q];
				center_box.max.v[q] = -halfsize.v[q];
			}
		}
		if ((planeNormal * center_box.min) + distance > 0.0f) return false;
		if ((planeNormal * center_box.max) + distance < 0.0f) return false;

		return true;
	}


	/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

	/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

	/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return false;

	bool TriangleBoxOverlap(const BoundingBox& box, Triangle & triangle)
	{

		Vector3 center = box.GetCenter();
		Vector3 halfSize = box.GetHalfSize();

		Vector3 v[3];
		v[0] = triangle.P1 - center;
		v[1] = triangle.P2 - center;
		v[2] = triangle.P3 - center;

		BoundingBox tr_aabb = GetMinimumBoundingBox(v, 3);

		// test triangle AABB agains AABB
		if (tr_aabb.min.x > halfSize.x || tr_aabb.max.x < -halfSize.x) return false;
		if (tr_aabb.min.y > halfSize.y || tr_aabb.max.y < -halfSize.y) return false;
		if (tr_aabb.min.z > halfSize.x || tr_aabb.max.z < -halfSize.x) return false;



		Vector3 e0 = v[1] - v[0];
		Vector3 e1 = v[2] - v[1];
		Vector3 plane_normal = e0.vectorProduct(e1).Normalise();
		float d = v[0] * plane_normal;

		// test triangle plane intersects bbx
		if (!Box_Plane_Overlap(box, plane_normal, d))
			return false;


		/*    3) */

		Vector3 e2 = v[2] - v[0];

		float min, max, p0, p1, p2, rad, fex, fey, fez;
		const unsigned int X = 0; const unsigned  int Y = 1; const unsigned  int Z = 2;

		float* v0 = v[0].v;
		float* v1 = v[1].v;
		float* v2 = v[2].v;

		fex = fabs(e0.x);
		fey = fabs(e0.y);
		fez = fabs(e0.z);

		float* boxhalfsize = halfSize.v;
		float* normal = plane_normal.v;
		AXISTEST_X01(e0.z, e0.y, fez, fey);
		AXISTEST_Y02(e0.z, e0.x, fez, fex);
		AXISTEST_Z12(e0.y, e0.x, fey, fex);

		fex = fabs(e1.x);
		fey = fabs(e1.y);
		fez = fabs(e1.z);
		AXISTEST_X01(e1.z, e1.y, fez, fey);
		AXISTEST_Y02(e1.z, e1.x, fez, fex);
		AXISTEST_Z0(e1.y, e1.x, fey, fex);


		fex = fabs(e2.x);
		fey = fabs(e2.y);
		fez = fabs(e2.z);
		AXISTEST_X2(e2.z, e2.y, fez, fey);
		AXISTEST_Y1(e2.z, e2.x, fez, fex);
		AXISTEST_Z12(e2.y, e2.x, fey, fex);

		return true;
	}
	bool Box_Sphere_Overlap(const BoundingBox & box, Sphere & sphere)
	{
		Vector3 closestPointInAabb = box.max.Min(box.min.Max(sphere.center));
		double distanceSquared = (closestPointInAabb - sphere.center).LenghtSq();

		// The AABB and the sphere overlap if the closest point within the rectangle is
		// within the sphere's radius
		return distanceSquared < (sphere.radius * sphere.radius);
	}

}