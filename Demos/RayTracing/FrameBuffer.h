#pragma once
#include <vector>
#include<Windows.h>
#include<algorithm>
#include "Camera.h"
#include "RayObjects.h"
#include "AndroUtils\Utils\ThreadPool.h"


class CFrameBuffer;
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

