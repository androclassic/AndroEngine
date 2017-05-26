
#include "FrameBuffer.h"
#include <math.h>
#include <algorithm>
#include "AndroUtils/Utils/MMath.h"
#include "AndroUtils/Utils/Ray.h"
#include "Scene.h"


#ifdef _USE_CUDA
#include "cuda_device_runtime_api.h"
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>

extern int FrameUpdateCuda(const Camera * pCamera, Object** objects, int nbObjects, unsigned long seed, unsigned int nbSamples, unsigned int blockSize);
extern unsigned int* AllocateBuffer(unsigned int w, unsigned int h, unsigned int blockSize, const Vector3& bgColour);

static int sBlockSize = 512;//todo
#endif


void RenderSliceTask::operator()()
{
	mfb->Render(m_scene, mRect);
}

//////////////////////////////////////////////////////////////////////////

void CFrameBuffer::Init(const int iWidth, const int iHeight, andro::Vector3 bgColour, andro::Vector3 cameraPos, andro::Vector3 cameraLook)
{


	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_camera = Camera(cameraPos, cameraLook, 90, afloat(iWidth) / iHeight, 2, 0.05f, 0, 0);
	m_bgColour = bgColour;

	m_FramebufferArray = new unsigned int[iWidth*iHeight];
	m_FramebufferTemp = new andro::Vector3[iWidth*iHeight];
	thread_pool = new andro::ThreadPool<RenderSliceTask>(8);


	memset(m_FramebufferArray, 0, iWidth*iHeight * sizeof(int));
	m_frameCount = 0;


#ifdef _USE_CUDA
	cudaFrameBuffer = AllocateBuffer(iWidth, iHeight, sBlockSize, m_bgColour);
#endif

}
CFrameBuffer::CFrameBuffer()
:m_camera (Vector3(), Vector3(), 90, afloat(640) / 480, 2, 0.05f, 0, 0)
{

}
CFrameBuffer::~CFrameBuffer()
{
	delete[] m_FramebufferArray;
	delete[] m_FramebufferTemp;
	delete thread_pool;
}

void CFrameBuffer::Clear()
{
	memset(m_FramebufferArray, 1, m_iWidth*m_iHeight * sizeof(int));
//	std::fill(m_FramebufferArray.begin(), m_FramebufferArray.end(), 0xFF);

}



andro::Vector3 CFrameBuffer::get_color(andro::ray& ray, const CScene* pScene, unsigned int depth)
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
		andro::ray_octree_traversal(pScene->GetOctree(), current_ray, objects, size);


#ifdef OBJECT_LIST_DEBUG_TEST
		for (std::vector<Object*>::const_iterator it = pScene->GetObjectsBegin(); it != pScene->GetObjectsEnd(); it++)
		{
			const Object* obj = *it;
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
//			emited = current_mat->emitted(0, 0, rec.point);
			const std::vector<const Object*>& light_objects = pScene->GetLights();

			{
				for (int l = 0; l < light_objects.size(); l++)
				{

					afloat light_closest_so_far = 99999.0f;
					andro::hit_record lrec, ltemp_rec;
					bool lhit = false;
					Object*  lobject = NULL;


					if (rec.object == light_objects[l]->m_shape )
					{
						emited = light_objects[l]->m_material->emitted(0, 0, rec.point);
						continue;
//						return final_colour + attenuation.Multiply(m_bgColour);
					}

					andro::Vector3 toLightDir = light_objects[l]->GetBoundingSphere().center - rec.point;
					float inv_rsq = 1.0 / toLightDir.LenghtSq();

					toLightDir = toLightDir + random_in_unit_sphere() * 0.3;
					toLightDir.NormalizeInto();
					andro::ray lightRay(rec.point, toLightDir);


					unsigned int size2 = 0;
					andro::ray_octree_traversal(pScene->GetOctree(), lightRay, objects2, size2);

					for (int k = 0; k < size2; k++)
					{
						Object* obj_l = objects2[k];
						if (obj_l->m_shape->hit(lightRay, 0.0001f, light_closest_so_far, ltemp_rec))
						{
							lhit = true;
							light_closest_so_far = ltemp_rec.t;
							lrec = ltemp_rec;
							lobject = obj_l;
						}
					}

					if (lhit && lrec.t > 0 && lobject == light_objects[l])
					{
						emited = emited + lobject->m_material->emitted(0,0,rec.point) * inv_rsq * max((toLightDir * rec.normal), 0);
					}
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


void CFrameBuffer::Update(const CScene* pScene, int numberOfSamples)
{

#ifdef _USE_CUDA
	FrameUpdateCuda(&m_camera, pScene->objectsVec, pScene->GetObjectsCount(), (afloat)GetTickCount() / 1000.0f, numberOfSamples, sBlockSize);
	cudaMemcpy(m_FramebufferArray, cudaFrameBuffer, sizeof(unsigned int) * m_iWidth * m_iHeight, cudaMemcpyDeviceToHost);

#else


	m_nbSamples = 1;
	Clear();


	const int num_jobs = 1024;
	std::thread t[num_jobs];

	//render
	for (int i = 0; i < num_jobs; i++)
	{
		Rect rect(0,0,1,1);
		rect.top = (1.0f / num_jobs) * (i);
		rect.bottom = (1.0f / num_jobs) * (i + 1);
		RenderSliceTask slice_task(*this, pScene, rect);

		thread_pool->Enqueue(slice_task);
	}
	thread_pool->FlushQueue();


	//static Rect full_screen(0, 0, 1, 1);
	//Render(octree, full_screen);
#endif

}
void CFrameBuffer::Render(const CScene* pScene, Rect& rect)
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
			afloat u = afloat(x + random_float(1)) / m_iWidth;
			afloat v = (afloat(y + random_float(1)) / m_iHeight);
			andro::ray r = m_camera.getRay(u, v);
			andro::Vector3 col = get_color(r, pScene, 10);
			color = color + col;
			color.Min(Vector3(1, 1, 1));


			m_FramebufferTemp[x + y * m_iWidth] = m_FramebufferTemp[x + y * m_iWidth] + color;
			andro::Vector3 currentCol = m_FramebufferTemp[x + y * m_iWidth] * (1.0 / m_frameCount);
			//gamma correction
			currentCol = andro::Vector3(sqrtf(currentCol.x), sqrtf(currentCol.y), sqrtf(currentCol.z));


			m_FramebufferArray[x + y * m_iWidth]  = int(currentCol.x * 255) << 16;
			m_FramebufferArray[x + y * m_iWidth] |= int(currentCol.y * 255) << 8;
			m_FramebufferArray[x + y * m_iWidth] |= int(currentCol.z * 255);
		}

	}
#endif
}
	
unsigned int* CFrameBuffer::GetFrameBuffer() const
{
	return /*static_cast<const unsigned int*>*/(&m_FramebufferArray[0]);
};


