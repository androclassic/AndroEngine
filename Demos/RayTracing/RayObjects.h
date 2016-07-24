#pragma once

#include "Material.h"
#include "AndroUtils\Utils\ThreadPool.h"
#include "AndroUtils\Utils\Shapes.h"
#include "AndroUtils\Utils\Octree.h"

struct Object
{
	virtual andro::Sphere GetBoundingSphere() const = 0;
	material* m_material;
	andro::Hitable*	m_shape;
};

struct SphereObject : public Object
{
	SphereObject(material* p_material, const Vector3& c, afloat r)
	{
		m_material = p_material;
		m_shape = new andro::Sphere(c, r);
	}

	~SphereObject()
	{
		delete m_shape;
		m_shape = nullptr;
	}

	inline andro::Sphere GetBoundingSphere() const
	{
		return *(andro::Sphere*)m_shape;
	}

};


struct TriangleObject : public Object
{
	TriangleObject(material* p_material, const Triangle& t)
	{
		m_material = p_material;
		m_shape = new andro::Triangle(t);
	}

	~TriangleObject()
	{
		delete m_shape;
		m_shape = nullptr;
	}


	inline andro::Sphere GetBoundingSphere() const
	{
		Triangle* t = (Triangle*)m_shape;
		return GetTriangleBoundingSphere(t);
	}

};




struct MeshObject : public Object
{
	MeshObject(material* p_material, const char* filename, const Vector3& c)
	{
		m_material = p_material;
		m_shape = new andro::Mesh(c, filename);
		m_center = c;
	}

	~MeshObject()
	{
		delete m_shape;
		m_shape = nullptr;
	}


	inline andro::Sphere GetBoundingSphere() const
	{
		//TODO
		return Sphere(m_center, ((andro::Mesh*)m_shape)->m_bounds.GetHalfSize().Lenght());
	}

	andro::Vector3 m_center;

};

struct BoxObject : public Object
{
	BoxObject(material* p_material, const Vector3& c, const Vector3& half_size)
		: m_center(c)
		, m_HalfSize(half_size)
	{
		m_material = p_material;
		m_shape = new andro::BoundingBox(c, half_size);
	}

	~BoxObject()
	{
		delete m_shape;
		m_shape = nullptr;
	}

	inline andro::Sphere GetBoundingSphere() const
	{
		return  andro::Sphere(m_center, m_HalfSize.Lenght());
	}

	Vector3 m_center;
	Vector3 m_HalfSize;
};

enum RectObjectType
{
	XY,
	XZ,
	YZ
};
struct RectObject : public Object
{

	RectObject(material* p_material, const Vector3& c, const Vector2& half_size, RectObjectType type)
	{
		m_material = p_material;
		m_center = c;
		m_halfSize = half_size;
		if (type == RectObjectType::XY)
			m_shape = new andro::xy_rect(c.x - half_size.x, c.x + half_size.x, c.y - half_size.y, c.y + half_size.y, c.z);
		else if (type == RectObjectType::XZ)
			m_shape = new andro::xz_rect(c.x - half_size.x, c.x + half_size.x, c.z - half_size.y, c.z + half_size.y, c.y);
		else
			m_shape = new andro::yz_rect(c.y - half_size.y, c.y + half_size.y, c.z - half_size.x, c.z + half_size.x, c.x);
	}

	~RectObject()
	{
		delete m_shape;
		m_shape = nullptr;
	}

	inline andro::Sphere GetBoundingSphere() const
	{
		return  andro::Sphere(m_center, m_halfSize.Lenght());
	}

	Vector2 m_halfSize;
	Vector3 m_center;
};

