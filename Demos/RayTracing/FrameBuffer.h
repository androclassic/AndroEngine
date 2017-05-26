#pragma once


#include <vector>
//#include<Windows.h>
#include<algorithm>
#include "Camera.h"
#include "RayObjects.h"
#include "AndroUtils/Utils/ThreadPool.h"
#include "AndroUtils/Utils/Singleton.h"

//#define OBJECT_LIST_DEBUG_TEST 1

class CFrameBuffer;
class CScene;

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
	RenderSliceTask(CFrameBuffer &fb, const CScene* pScene, const Rect& subRect)
	{
		mfb = &fb;
		m_scene = pScene;
		mRect = subRect;
	}
	void operator()();

	CFrameBuffer* mfb;
	const CScene* m_scene;
	Rect mRect;

};

class CFrameBuffer
{
public:
	SINGLETON(CFrameBuffer);
		void Init(const int iWidth, const int iHeight, andro::Vector3  bgColour, andro::Vector3 cameraPos, andro::Vector3 cameraLook);

	void Clear();
	void Update(const CScene* pScene, int numberOfSamples);
	void Render(const CScene* pScene, Rect& rect);


	unsigned int* GetFrameBuffer() const;
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

	int GetValue(int index) { return m_FramebufferArray[index]; }
	const Camera* GetCamera() const { return &m_camera;  }


	unsigned int m_frameCount;
private:
	andro::Vector3 get_color( andro::ray& ray, const CScene* pScene, unsigned int depth = 0);
	int	m_nbSamples;

	unsigned int* m_FramebufferArray;
	andro::Vector3* m_FramebufferTemp;

	int m_iWidth, m_iHeight;
	Camera m_camera;
	andro::ThreadPool<RenderSliceTask>* thread_pool;
	andro::Vector3 m_bgColour;

#ifdef _USE_CUDA
	unsigned int* cudaFrameBuffer = 0;
#endif

};

