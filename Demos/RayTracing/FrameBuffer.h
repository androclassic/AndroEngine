#pragma once
#include <vector>
#include<Windows.h>
#include<algorithm>
#include "Camera.h"
#include "Material.h"
#include "AndroUtils\Utils\ThreadPool.h"

struct Object
{
	Object(material* p_material, const Vector3& c, float r)
		: m_material(p_material)
		, m_shape(c, r)
	{}


	const material* m_material;
	andro::Sphere	m_shape;

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

class CFrameBuffer;

struct RenderSliceTask
{
	RenderSliceTask() {};
	RenderSliceTask(CFrameBuffer &fb, const std::vector<Object>& objects, const Rect& subRect)
	{
		mfb = &fb;
		m_objects = &objects;
		mRect = subRect;
	}
	void operator()();

	CFrameBuffer* mfb;
	const std::vector<Object>* m_objects;
	Rect mRect;

};

class CFrameBuffer
{
public:
	CFrameBuffer( const int iWidth, const int iHeight );
	~CFrameBuffer();

	void Clear();
	void Update(const std::vector<Object>& objects);
	void Render(const std::vector<Object>& objects, Rect& rect);


	const unsigned int* GetFrameBuffer() const;
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }

	int GetValue(int index) { return m_FramebufferArray[index]; }
	
private:
	andro::Vector3 get_color(const andro::ray& ray, const std::vector<Object>& objects, unsigned int depth = 0);


	std::vector<unsigned int> m_FramebufferArray;
	int m_iWidth, m_iHeight;
	Camera m_camera;
	andro::ThreadPool<RenderSliceTask> thread_pool;

};

