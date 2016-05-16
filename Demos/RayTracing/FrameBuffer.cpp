
#include "FrameBuffer.h"
#include <math.h>
#include <algorithm>
#include "AndroUtils\Utils\MMath.h"
#include "AndroUtils\Utils\Ray.h"


//////////////////////////////////////////////////////////////////////////

CFrameBuffer::CFrameBuffer( const int iWidth, const int iHeight )
	:m_iWidth(iWidth), m_iHeight(iHeight), m_camera(float(iWidth)/iHeight)
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

andro::Vector3 CFrameBuffer::get_color(const andro::ray& ray, const std::vector<andro::Hitable*>& objects)
{

	andro::hit_record rec, temp_rec;
	bool hit = false;
	float closest_so_far = 99999.0f;
	for (auto hitable : objects)
	{
		if (hitable->hit(ray, 0, closest_so_far, temp_rec))
		{
			hit = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	if (hit && rec.t > 0)
	
	{
		return (andro::Vector3(1, 1, 1) + rec.normal) * 0.5;
	}

	andro::Vector3 unit_dir = ray.dir;
	unit_dir.Normalize();
	
	 
	return   andro::Vector3(0.5, 0.7, 1.0);
 }

void CFrameBuffer::Render(const std::vector<andro::Hitable*>& objects)
{

	float ratio = m_iWidth / m_iHeight;


	for (int y = 0; y < m_iHeight; y++)
	{
		for (unsigned int x = 0; x < m_iWidth; x++)
		{
			float u = float(x) / m_iWidth;
			float v =  (float(y) / m_iHeight);
			andro::ray r = m_camera.getRay(u, v);
			andro::Vector3 color = get_color(r, objects);

			m_FramebufferArray[x + y * m_iWidth]  = int(color.x * 255)  <<16;
			m_FramebufferArray[x + y * m_iWidth] |= int(color.y * 255)  << 8;
			m_FramebufferArray[x + y * m_iWidth] |= int(color.z * 255);
		}

	}
}
	
const unsigned int* CFrameBuffer::GetFrameBuffer() const
{
	return static_cast<const unsigned int*>(&m_FramebufferArray[0]);
};


