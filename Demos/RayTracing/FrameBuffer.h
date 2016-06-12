#pragma once
#include <vector>
#include<Windows.h>
#include<algorithm>
#include "Camera.h"
#include "Material.h"
#include "AndroUtils\Utils\ThreadPool.h"
#include "AndroUtils\Utils\Shapes.h"
#include "AndroUtils\Utils\Octree.h"

class CFrameBuffer;



struct Object
{
//	virtual andro::BoundingBox GetAABB() const = 0 ;
	virtual andro::Sphere GetBoundingSphere() const = 0;
	const material* m_material;
	andro::Hitable*	m_shape;
};

struct SphereObject : public Object
{
	SphereObject(material* p_material, const Vector3& c, float r)
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



struct Rect 
{
	Rect()
	{
		left = right = top = bottom = 0;
	}
	Rect(float l, float t, float r, float b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}

	float left, top, right, bottom;
};

struct RenderSliceTask
{
	RenderSliceTask() {};
	RenderSliceTask(CFrameBuffer &fb, const andro::OctreeNode<Object*>const* octree, const Rect& subRect)
	{
		mfb = &fb;
		m_octree = octree;
		mRect = subRect;
	}
	void operator()();

	CFrameBuffer* mfb;
	const andro::OctreeNode<Object*>const* m_octree;
	Rect mRect;

};
//#define OBJECT_LIST_DEBUG_TEST 1

class CFrameBuffer
{
public:
	CFrameBuffer( const int iWidth, const int iHeight );
	~CFrameBuffer();

	void Clear();
	void Update(const andro::OctreeNode<Object*>const* octree);
	void Render(const andro::OctreeNode<Object*>const* octree, Rect& rect);


	const unsigned int* GetFrameBuffer() const;
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

	int GetValue(int index) { return m_FramebufferArray[index]; }

#ifdef OBJECT_LIST_DEBUG_TEST
	std::vector<Object*> debug_objects;
#endif

private:
	andro::Vector3 get_color( andro::ray& ray, const andro::OctreeNode<Object*>const* octree, unsigned int depth = 0);


	std::vector<unsigned int> m_FramebufferArray;
	int m_iWidth, m_iHeight;
	Camera m_camera;
	andro::ThreadPool<RenderSliceTask> thread_pool;


};

