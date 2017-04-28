#pragma once


#include <vector>
//#include<Windows.h>
#include<algorithm>
#include "Camera.h"
#include "RayObjects.h"
#include "AndroUtils/Utils/ThreadPool.h"

//#define OBJECT_LIST_DEBUG_TEST 1

class CFrameBuffer;
struct Rect
{
	Rect()
	{
		left = right = top = bottom = 0;
	}
	Rect(afloat l, afloat t, afloat r, afloat b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}

	afloat left, top, right, bottom;
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
	CFrameBuffer(const int iWidth, const int iHeight, andro::Vector3  bgColour, andro::Vector3 cameraPos, andro::Vector3 cameraLook);
	~CFrameBuffer();

	void Clear();
	void Update(const andro::OctreeNode<Object*>const* octree, int numberOfSamples);
	void Render(const andro::OctreeNode<Object*>const* octree, Rect& rect);


	unsigned int* GetFrameBuffer() const;
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

	int GetValue(int index) { return m_FramebufferArray[index]; }
	const Camera* GetCamera() const { return &m_camera;  }

#ifdef OBJECT_LIST_DEBUG_TEST
	std::vector<Object*> debug_objects;
#endif
	void AddLight(Object* obj) { light_objects.push_back(obj); }

	unsigned int m_frameCount;
private:
	andro::Vector3 get_color( andro::ray& ray, const andro::OctreeNode<Object*>const* octree, unsigned int depth = 0);
	int	m_nbSamples;

	unsigned int* m_FramebufferArray;
	andro::Vector3* m_FramebufferTemp;

	int m_iWidth, m_iHeight;
	Camera m_camera;
	andro::ThreadPool<RenderSliceTask> thread_pool;
	andro::Vector3 m_bgColour;

	std::vector<Object*> light_objects;

};

