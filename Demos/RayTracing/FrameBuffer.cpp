
#include "FrameBuffer.h"
#include <math.h>
#include <algorithm>
#include "AndroUtils/Utils/MMath.h"
#include "AndroUtils/Utils/Ray.h"


void RenderSliceTask::operator()()
{
	mfb->Render(m_octree, mRect);
}

//////////////////////////////////////////////////////////////////////////

CFrameBuffer::CFrameBuffer(const int iWidth, const int iHeight, andro::Vector3 bgColour, andro::Vector3 cameraPos, andro::Vector3 cameraLook)
:m_iWidth(iWidth)
, m_iHeight(iHeight)
, m_camera(cameraPos, cameraLook, 90, afloat(iWidth) / iHeight, 2, 0.05f, 0, 0)
, thread_pool(8)
, m_bgColour(bgColour)
{
	m_FramebufferArray = new unsigned int[iWidth*iHeight];
	memset(m_FramebufferArray,0 ,iWidth*iHeight * sizeof(int));

}

CFrameBuffer::~CFrameBuffer()
{
	delete[] m_FramebufferArray;
}

void CFrameBuffer::Clear()
{
	memset(m_FramebufferArray, 1, m_iWidth*m_iHeight * sizeof(int));
//	std::fill(m_FramebufferArray.begin(), m_FramebufferArray.end(), 0xFF);

}



andro::Vector3 CFrameBuffer::get_color(andro::ray& ray, const andro::OctreeNode<Object*>const * octree, unsigned int depth)
{

	Object* objects[50000];
	Object* objects2[50000];
	uint16_t nbBounce = depth;
	andro::ray current_ray = ray;
	andro::Vector3 attenuation(1);
	andro::Vector3 final_colour(0.0);
#ifndef _USE_CUDA
	for (uint16_t bounce = 0; bounce < nbBounce; bounce++)
	{

		afloat closest_so_far = 99999.0f;
		andro::hit_record rec, temp_rec;
		bool hit = false;
		const material* current_mat = nullptr;
		unsigned int size = 0;
		current_ray.dir.NormalizeInto();
		andro::ray_octree_traversal(octree, current_ray, objects, size);


#ifdef OBJECT_LIST_DEBUG_TEST
		for (auto obj : debug_objects){
#else
		for (int i = 0; i < size; i++)
		{
			Object* obj = objects[i];
#endif
			if (obj->m_shape->hit(current_ray, 0.0001f, closest_so_far, temp_rec))
			{
				hit = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
				current_mat = obj->m_material;
			}
		}

		if (hit && rec.t > 0)
		{
			andro::Vector3  color;
			andro::Vector3  emited;

			{
				for (int l = 0; l < light_objects.size(); l++)
				{

					//if (rec.object == light_objects[l]->m_shape && bounce == 0)
					//{
					//	emited = current_mat->emitted(0, 0, rec.point);
					//	continue;
					//}

					afloat light_closest_so_far = 99999.0f;
					afloat light_distance_t = 99999.0f;

					andro::Vector3 toLightDir = light_objects[l]->GetBoundingSphere().center - rec.point;
					float inv_rsq = 1.0 / toLightDir.LenghtSq();
					toLightDir.NormalizeInto();
					andro::ray lightRay(rec.point, toLightDir);
					andro::hit_record hit;
					Vector3 emittedTemp;

					unsigned int size2 = 0;
					andro::ray_octree_traversal(octree, lightRay, objects2, size2);

					for (int k = 0; k < size2; k++)
					{
						Object* obj = objects2[k];
						if (obj->m_shape->hit(lightRay, 0.0001f, light_closest_so_far, hit))
						{
							light_closest_so_far = hit.t;
							if (hit.object == light_objects[l]->m_shape)
							{
								emittedTemp = light_objects[l]->m_material->emitted(0, 0, rec.point);
								light_distance_t = light_closest_so_far;
							}
						}
					}

					if (light_distance_t != light_closest_so_far)
					{
						emittedTemp = Vector3();
					}
					emited = emited + emittedTemp * inv_rsq * (toLightDir * rec.normal);
				}
			}

			final_colour = final_colour + attenuation.Multiply(emited);


			andro::Vector3  new_att;
			andro::ray new_ray;
			bool scatter = current_mat->scatter(current_ray, rec, new_att, new_ray);

			if (scatter)
			{
				current_ray = new_ray;
				attenuation = attenuation.Multiply(new_att);
				if (attenuation.LenghtSq() < 0.0025)
					return final_colour;
			}
			else
				return final_colour;

		}
		else
			return final_colour + attenuation.Multiply( m_bgColour);
	}
#endif
	return final_colour;
 }


void CFrameBuffer::Update(const andro::OctreeNode<Object*>const* octree, int numberOfSamples)
{
	m_nbSamples = numberOfSamples;
	Clear();


	const int num_jobs = 1024;
	std::thread t[num_jobs];

	//render
	for (int i = 0; i < num_jobs; i++)
	{
		Rect rect(0,0,1,1);
		rect.top = (1.0f / num_jobs) * (i);
		rect.bottom = (1.0f / num_jobs) * (i + 1);
		RenderSliceTask slice_task(*this, octree, rect);

		thread_pool.Enqueue(slice_task);
	}
	thread_pool.FlushQueue();

	//static Rect full_screen(0, 0, 1, 1);
	//Render(octree, full_screen);

}
void CFrameBuffer::Render(const andro::OctreeNode<Object*>const* octree, Rect& rect)
{
#ifndef _USE_CUDA
	afloat ratio = m_iWidth / m_iHeight;

	unsigned int ns = m_nbSamples;

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
				afloat u = afloat(x + random_float(1)) / m_iWidth;
				afloat v = (afloat(y + random_float(1)) / m_iHeight);
				andro::ray r = m_camera.getRay(u, v);
				andro::Vector3 col = get_color(r, octree, 10);
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
#endif
}
	
unsigned int* CFrameBuffer::GetFrameBuffer() const
{
	return /*static_cast<const unsigned int*>*/(&m_FramebufferArray[0]);
};


