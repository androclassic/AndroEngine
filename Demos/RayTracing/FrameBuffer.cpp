
#include "FrameBuffer.h"
#include <math.h>
#include <algorithm>
#include "AndroUtils\Utils\MMath.h"
#include "AndroUtils\Utils\Ray.h"



void RenderSliceTask::operator()()
{
	mfb->Render(*m_objects, mRect);
}

//////////////////////////////////////////////////////////////////////////

CFrameBuffer::CFrameBuffer( const int iWidth, const int iHeight )
	:m_iWidth(iWidth), m_iHeight(iHeight), m_camera(andro::Vector3(7,1.5,2.5), andro::Vector3(0.0f, 0.0f, -1.0f),90,float(iWidth)/iHeight, 6,0.04f)
	, thread_pool(8)
{
	m_FramebufferArray.resize(iWidth*iHeight,0);

}

CFrameBuffer::~CFrameBuffer()
{
}

void CFrameBuffer::Clear()
{
//	memset(&m_FramebufferArray[0], 0x0,m_iWidth*m_iHeight*4);
	std::fill(m_FramebufferArray.begin(), m_FramebufferArray.end(), 0xFF);

}



andro::Vector3 CFrameBuffer::get_color(const andro::ray& ray, const std::vector<Object>& objects, unsigned int depth)
{

	andro::hit_record rec, temp_rec;
	bool hit = false;
	float closest_so_far = 99999.0f;
	const material* current_mat = nullptr;

	for (auto& obj : objects)
	{
		
		if (obj.m_shape.hit(ray, 0.0001f, closest_so_far, temp_rec))
		{
			hit = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
			current_mat = obj.m_material;
		}
	}

	if (hit && rec.t > 0)
	{
		andro::Vector3  color;
		andro::Vector3 attenuation;
		andro::ray new_ray;
		if (depth < 40 && current_mat->scatter(ray, rec, attenuation, new_ray))
		{

			color = get_color(new_ray, objects, ++depth);
			color.x *=attenuation.x;
			color.y *=attenuation.y;
			color.z *=attenuation.z;
		}

		return color;
	}

	Vector3 unit_v = ray.dir;
	unit_v.Normalize();
	float t = 0.5 * (unit_v.y + 1.0f);

	return   andro::Vector3(0.1, 0.1,0.3) * (1.0f - t) + andro::Vector3(0.002, 0.002,0.004) * t;
 }


void CFrameBuffer::Update(const std::vector<Object>& objects)
{
	Clear();


	const int num_jobs = 1024;
	std::thread t[num_jobs];

	//render
	for (int i = 0; i < num_jobs; i++)
	{
		Rect rect(0,0,1,1);
		rect.top = (1.0f / num_jobs) * (i);
		rect.bottom = (1.0f / num_jobs) * (i + 1);

		thread_pool.Enqueue(RenderSliceTask(*this, objects, rect));
	}
	thread_pool.FlushQueue();

	//static Rect full_screen(0, 0, 1, 1);
	//Render(objects, full_screen);

}
void CFrameBuffer::Render(const std::vector<Object>& objects, Rect& rect)
{

	float ratio = m_iWidth / m_iHeight;

	unsigned int ns = 1;
	unsigned int start_x = rect.left * m_iWidth;
	unsigned int start_y = rect.top * m_iHeight;
	unsigned int end_x = rect.right * m_iWidth;
	unsigned int end_y = rect.bottom * m_iHeight;

	for (int y = start_y; y < end_y; y++)
	{
		for (unsigned int x = start_x; x < end_x; x++)
		{
			andro::Vector3 color;
			for (unsigned int s = 0; s < ns; s++)
			{
				float u = float(x + random_float(1)) / m_iWidth;
				float v = (float(y + random_float(1)) / m_iHeight);
				andro::ray r = m_camera.getRay(u, v);
				andro::Vector3 col = get_color(r, objects);
				color = color + col;
			}
			color = color * (1.0f / ns);

			//gamma correction
			color = andro::Vector3(sqrtf(color.x), sqrtf(color.y), sqrtf(color.z));

			m_FramebufferArray[x + y * m_iWidth] = int(color.x * 255) << 16;
			m_FramebufferArray[x + y * m_iWidth] |= int(color.y * 255) << 8;
			m_FramebufferArray[x + y * m_iWidth] |= int(color.z * 255);
		}

	}
}
	
const unsigned int* CFrameBuffer::GetFrameBuffer() const
{
	return static_cast<const unsigned int*>(&m_FramebufferArray[0]);
};


