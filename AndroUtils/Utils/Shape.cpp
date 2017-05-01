#include <string.h>
#include "Shapes.h"
#include "Octree.h"
#include "Trace.h"
#include "AndroUtils.h"
#include "../../External/tinyOBJ/tiny_obj_loader.h"
#include "../../Renderer/Log.h"

namespace andro
{
#if defined(_USE_CUDA) && !defined(ANDRO_UTILS) ||  !defined(_USE_CUDA) 

	DEVICE_HOST Mesh::Mesh(const Vector3& c, const char* filename) :center(c)
	{

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, filename, NULL, TRUE);

		if (!err.empty()) {
			LOG_MSG(err);
		}

		if (!ret) {
			return;
		}


		//Read vertices and indices
		std::vector<andro::Vector3> vertices;
		std::vector<unsigned int> indices;
		m_bounds.max = andro::Vector3(-999, -999, -999);
		m_bounds.min = andro::Vector3(999, 999, 999);

		unsigned int base = 0;
		for (auto shape : shapes)
		{
			for (int i = 0, n = 0, t = 0; i < shape.mesh.positions.size(); i += 3, t += 2, n += 3)
			{
				andro::Vector3 p;

				m_bounds.min.x = (std::min)(shape.mesh.positions[i],	 (float)m_bounds.min.x);
				m_bounds.min.y = (std::min)(shape.mesh.positions[i + 1], (float)m_bounds.min.y);
				m_bounds.min.z = (std::min)(shape.mesh.positions[i + 2], (float)m_bounds.min.z);

				m_bounds.max.x = (std::max)(shape.mesh.positions[i],	 (float)m_bounds.max.x);
				m_bounds.max.y = (std::max)(shape.mesh.positions[i + 1], (float)m_bounds.max.y);
				m_bounds.max.z = (std::max)(shape.mesh.positions[i + 2], (float)m_bounds.max.z);

				p = andro::Vector3(shape.mesh.positions[i], shape.mesh.positions[i + 1], shape.mesh.positions[i + 2]);
				vertices.push_back(p);
			}
			for (int i = 0; i < shape.mesh.indices.size(); i++)
			{
				indices.push_back(base + shape.mesh.indices[i]);
			}

			base += (shape.mesh.positions.size() / 3);
		}

		for (int i = 0; i < indices.size() - 2; i++)
		{
			andro::Triangle t;
			t.P1 = vertices[indices[i]];
			t.P2 = vertices[indices[i + 1]];
			t.P3 = vertices[indices[i + 2]];

			m_triangles.push_back(t);
		}

		std::vector<Triangle*> triangles;
		for (auto& t : m_triangles)
		{
			triangles.push_back(&t);
		}


		// build octree of the scene
		//TODO custom octree step 
		m_octree = andro::BuildOctree<Triangle*>(triangles, m_bounds, 4, [](const andro::BoundingBox& box, Triangle* t)	{ return TriangleBoxOverlap(box, *t); }, true);

	}

	DEVICE_HOST Vector2 Mesh::getUV(const Vector3& point) const
	{
		//TODO
		Vector2 uv;
		return uv;
	}

	DEVICE_HOST bool Mesh::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{
		Triangle* objects[500];

		ray _ray = r;
		_ray.origin = _ray.origin - center;

		andro::hit_record temp_rec;
		bool hit = false;
		afloat closest_so_far = t_max;

		unsigned int size = 0;
		OctreeNode<Triangle*>* octree = (OctreeNode<Triangle*>*)m_octree;
		andro::ray_octree_traversal(octree, _ray, objects, size);

		for (int i = 0; i < size; i++)
		{
			Triangle* t = objects[i];
			if (t->hit(_ray, t_min, closest_so_far, temp_rec))
			{
				hit = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit;
	}




	DEVICE_HOST Vector2 Triangle::getUV(const Vector3& point) const
	{
		//TODO
		Vector2 uv;
		return uv;
	}

	DEVICE_HOST bool Triangle::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{
		static float eps = 0.000001f;

		andro::Vector3 e1 = P2 - P1;
		andro::Vector3 e2 = P3 - P1;
		andro::Vector3 q = r.dir.vectorProduct(e2);
		float a = e1 * q;
		if (a > -eps && a < eps)
			return false;

		float f = 1.0f / a;

		andro::Vector3 s = r.origin - P1;
		float u = f * (s * q);

		if (u < 0.0)
			return false;

		andro::Vector3 _r = s.vectorProduct(e1);

		float v = f * (r.dir * _r);
		if (v < 0.0 || v+u > 1.0)
			return false;

		float t  = f * (e2 * _r);

		if (t< t_min || t > t_max)
			return false;

		rec.normal = e1.vectorProduct(e2);
		rec.normal.NormalizeInto();
		rec.t = t;
		rec.point = r.get_point_at(t);
		rec.uv = getUV(rec.point);


		return true;
	}

	DEVICE_HOST Vector2 Sphere::getUV(const Vector3& point) const
	{
		Vector2 uv;
		afloat phi = atan2f(point.z, point.x);
		afloat theta = asinf(point.y);
		uv.x = 1 - (phi + _PI_) / (2 * _PI_);
		uv.y = (theta + _PI_ / 2.0f) / _PI_;

		return uv;
	}

	DEVICE_HOST bool Sphere::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{

		// p(t) = (origin + t * dir)) - center)
		//dot( ( p(t), p(t)) = R* R

		Vector3 oc = r.origin - center;
		afloat a = r.dir * r.dir;
		afloat b = (oc * r.dir);
		afloat c = (oc * oc) - (radius * radius);

		afloat discriminant = b*b - a*c;

		if (discriminant < 0)
			return false;

		afloat temp = (-b - sqrtf(discriminant) / a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.point = r.get_point_at(temp);
			rec.normal = (rec.point - center) * (1.0f / radius);
			rec.object = this;
			rec.uv = getUV(rec.point);
			return true;
		}

		temp = (-b + sqrtf(discriminant) / a);
		if (temp > t_min && temp < t_max)
		{
			rec.t = temp;
			rec.point = r.get_point_at(temp);
			rec.normal = (rec.point - center) * (1.0f / radius);
			rec.object = this;
			rec.uv = getUV(rec.point);
			return true;
		}

		return false;
	}




	DEVICE_HOST BoundingBox::BoundingBox()
	{
		min = andro::Vector3(99999.0f, 99999.0f, 99999.0f);
		max = andro::Vector3(-99999.0f, -99999.0f, -99999.0f);
	}

	DEVICE_HOST Vector3 BoundingBox::GetHalfSize() const
	{
		Vector3 halfSize = (min + max)*0.5f - min;
		return halfSize;
	}

	DEVICE_HOST Vector3 BoundingBox::GetCenter() const
	{
		return (min + max)*0.5f;
	}


	DEVICE_HOST BoundingBox GetMinimumBoundingBox(Vector3* points, unsigned int num)
	{
		BoundingBox bbx;
		for (unsigned int i = 0; i < num; i++)
		{
			bbx.min.x = (std::min)(points[i].x, bbx.min.x);
			bbx.min.y = (std::min)(points[i].y, bbx.min.y);
			bbx.min.z = (std::min)(points[i].z, bbx.min.z);

			bbx.max.x = (std::max)(points[i].x, bbx.max.x);
			bbx.max.y = (std::max)(points[i].y, bbx.max.y);
			bbx.max.z = (std::max)(points[i].z, bbx.max.z);

		}
		return bbx;
	}

	DEVICE_HOST bool BoundingBox::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{


		for (int a = 0; a < 3; a++)
		{
			afloat t0 = fminf((min.v[a] - r.origin.v[a]) / r.dir.v[a],
				(max.v[a] - r.origin.v[a]) / r.dir.v[a]);

			afloat t1 = fmaxf((min.v[a] - r.origin.v[a]) / r.dir.v[a],
				(max.v[a] - r.origin.v[a]) / r.dir.v[a]);

			t_min = fmaxf(t0, t_min);
			t_max = fminf(t1, t_max);
			if (t_max <= t_min)
				return false;

		}

		Vector3 hit_point = r.get_point_at(t_min);
		Vector3 normal(0, 1, 1);
		static afloat delta = 0.005f;

		if (fabs(hit_point.x - min.x) < delta)
			normal = Vector3(-1, 0, 0);
		else if (fabs(hit_point.x - max.x) < delta)
			normal = Vector3(1, 0, 0);
		else if (fabs(hit_point.z - max.z) < delta)
			normal = Vector3(0, 0, 1);
		else if (fabs(hit_point.z - min.z) < delta)
			normal = Vector3(0, 0, -1);
		else if (fabs(hit_point.y - min.y) < delta)
			normal = Vector3(0, -1, 0);
		else if (fabs(hit_point.y - max.y) < delta)
			normal = Vector3(0, 1, 0);
		else
			ASSERT(false, "DELTA not good");

		rec.t = t_min;
		rec.point = hit_point;
		rec.normal = normal;
		rec.object = this;
		rec.uv = getUV(rec.point);

		return true;
	}


//--------------------------------------------------------------------------------------------------
	DEVICE_HOST bool xy_rect::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{
		afloat t = (k - r.origin.z) / r.dir.z;
		if (t < t_min || t > t_max)
			return false;

		afloat x = r.origin.x + t * r.dir.x;
		afloat y = r.origin.y + t * r.dir.y;

		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;

		rec.point = r.get_point_at(t);
		rec.t = t;
		rec.normal = Vector3(0, 0, n);
		rec.uv = getUV(rec.point);
		rec.object = this;
		return true;
	}

	DEVICE_HOST Vector2 xy_rect::getUV(const Vector3& p) const
	{
		return Vector2((p.x - x0)/ (x1-x0), (p.y - y0)/(y1 - y0));
	}
	//--------------------------------------------------------------------------------------------------
	DEVICE_HOST bool xz_rect::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{
		afloat t = (k - r.origin.y) / r.dir.y;
		if (t < t_min || t > t_max)
			return false;

		afloat x = r.origin.x + t * r.dir.x;
		afloat z = r.origin.z + t * r.dir.z;

		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;

		rec.point = r.get_point_at(t);
		rec.t = t;
		rec.normal = Vector3(0, n, 0);
		rec.uv = getUV(rec.point);
		rec.object = this;
		return true;
	}

	DEVICE_HOST Vector2 xz_rect::getUV(const Vector3& p) const
	{
		return Vector2((p.x - x0) / (x1 - x0), (p.z - z0) / (z1 - z0));
	}
	//--------------------------------------------------------------------------------------------------
	DEVICE_HOST bool yz_rect::hit(const ray& r, afloat t_min, afloat t_max, hit_record& rec) const
	{
		afloat t = (k - r.origin.x) / r.dir.x;
		if (t < t_min || t > t_max)
			return false;

		afloat y = r.origin.y + t * r.dir.y;
		afloat z = r.origin.z + t * r.dir.z;

		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;

		rec.point = r.get_point_at(t);
		rec.t = t;
		rec.normal = Vector3(n, 0, 0);
		rec.uv = getUV(rec.point);
		rec.object = this;
		return true;
	}

	DEVICE_HOST Vector2 yz_rect::getUV(const Vector3& p) const
	{
		return Vector2((p.y - y0) / (y1 - y0), (p.z - z0) / (z1 - z0));
	}

	//------------------------------------------------------------------
	//intersections
	DEVICE_HOST bool  Box_Plane_Overlap(const BoundingBox& box, Vector3 planeNormal, afloat distance)
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

	DEVICE_HOST bool TriangleBoxOverlap(const BoundingBox& box, Triangle & triangle)
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
		afloat d = v[0] * plane_normal;

		// test triangle plane intersects bbx
		if (!Box_Plane_Overlap(box, plane_normal, d))
			return false;


		/*    3) */

		Vector3 e2 = v[2] - v[0];

		afloat min, max, p0, p1, p2, rad, fex, fey, fez;
		const unsigned int X = 0; const unsigned  int Y = 1; const unsigned  int Z = 2;

		afloat* v0 = v[0].v;
		afloat* v1 = v[1].v;
		afloat* v2 = v[2].v;

		fex = fabs(e0.x);
		fey = fabs(e0.y);
		fez = fabs(e0.z);

		afloat* boxhalfsize = halfSize.v;
		afloat* normal = plane_normal.v;
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
	DEVICE_HOST bool Box_Sphere_Overlap(const BoundingBox & box, Sphere & sphere)
	{
		Vector3 closestPointInAabb = box.max.Min(box.min.Max(sphere.center));
		double distanceSquared = (closestPointInAabb - sphere.center).LenghtSq();

		// The AABB and the sphere overlap if the closest point within the rectangle is
		// within the sphere's radius
		return distanceSquared < (sphere.radius * sphere.radius);
	}



	//---------------------------------------------------------------------------------------

	DEVICE_HOST Sphere GetTriangleBoundingSphere(const Triangle* t)
	{
		// Calculate relative distances
		float A = (t->P2 - t->P1).Lenght();
		float B = (t->P2 - t->P3).Lenght();
		float C = (t->P3 - t->P1).Lenght();

		// Re-orient triangle (make A longest side)
		const andro::Vector3 *a = &t->P3, *b = &t->P1, *c = &t->P2;
		if (B < C)
		{
			//swap B C
			float temp = B;
			B = C;
			C = temp;

			//swap b c
			b = &t->P2;
			c = &t->P1;
		}
		if (A < B)
		{
			//swap A B
			float temp = A;
			A = B;
			B = temp;

			const andro::Vector3 * tem = a;
			a = b;
			b = tem;
		}

		// If obtuse, just use longest diameter, otherwise circumscribe
		if ((B*B) + (C*C) <= (A*A))
		{
			float radius = A / 2.f;
			andro::Vector3 position = (*b + *c) * (1.0 / 2.f);
			return andro::Sphere(position, radius);
		}
		else {
			// http://en.wikipedia.org/wiki/Circumscribed_circle
			float cos_a = (B*B + C*C - A*A) / (B*C * 2);
			float radius = A / (sqrt(1 - cos_a*cos_a)*2.f);
			andro::Vector3 alpha = *a - *c, beta = *b - *c;
			andro::Vector3 position = (beta *(alpha * alpha) - alpha * (beta*beta)).vectorProduct(alpha.vectorProduct(beta))* (1.0 / (alpha.vectorProduct(beta)*(alpha.vectorProduct(beta)) * 2.f)) + *c;

			return andro::Sphere(position, radius);

		}
	}

#endif //#if defined(_USE_CUDA) && !defined(ANDRO_UTILS) ||  !defined(_USE_CUDA) 
	
	//-------------------------------------------------------------------------------
	void CreateCubeModel(const BoundingBox& aabb, Vector3 points[8], unsigned int  indices[36])
	{

		andro::Vector3 center = aabb.GetCenter();
		afloat half_x = aabb.GetHalfSize().x;
		afloat half_y = aabb.GetHalfSize().y;
		afloat half_z = aabb.GetHalfSize().z;


		/* init_resources */
		unsigned int cube_elements[] = {
			// front
			0, 1, 2,
			2, 3, 0,
			// top
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// bottom
			4, 0, 3,
			3, 7, 4,
			// left
			4, 5, 1,
			1, 0, 4,
			// right
			3, 2, 6,
			6, 7, 3,
		};

		//front
		points[0] = center + andro::Vector3(-half_x, -half_y, half_z);
		points[1] = center + andro::Vector3(half_x, -half_y, half_z);
		points[2] = center + andro::Vector3(half_x, half_y, half_z);
		points[3] = center + andro::Vector3(-half_x, half_y, half_z);
		//back
		points[4] = center + andro::Vector3(-half_x, -half_y, -half_z);
		points[5] = center + andro::Vector3(half_x, -half_y, -half_z);
		points[6] = center + andro::Vector3(half_x, half_y, -half_z);
		points[7] = center + andro::Vector3(-half_x, half_y, -half_z);

#ifdef _WIN32
		memcpy_s(indices, sizeof(cube_elements), cube_elements, sizeof(cube_elements));
#else
		memcpy(indices, cube_elements, sizeof(cube_elements));
#endif		
	}

}