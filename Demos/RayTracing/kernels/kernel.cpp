#include "RayTracing\FrameBuffer.h"
#ifdef _USE_CUDA

#include <iostream>
#include <math.h>

#include <windows.h>
#include <windowsx.h>
#include <fstream>
#include <sstream>


#include "RayTracing\Camera.h"
#include "AndroUtils/Utils/Ray.h"
#include "RayTracing\Material.h"
#include "RayTracing\texture.h"
#include "RayTracing\RayObjects.h"
#include <curand_kernel.h>

#include "RayTracing\Camera.h"
#include "RayTracing\ObjectsLuaDesc.h"

#include "AndroUtils\Utils\AndroUtils.cpp"
#include "AndroUtils\Utils\MMath.cpp"
#include "AndroUtils\Utils\Shape.cpp"

using namespace andro;
typedef Vector3 _node;

static int width, height;
DEVICE curandState_t* g_statesDevice;
DEVICE Vector3*		  g_BgColourDevice;

static Camera* cudaCamera = 0;
static Vector3* g_BgColour = 0;
static curandState_t* g_states = 0;

Vector3* frameBuffer = NULL;
unsigned int* intFrameBuffer = NULL;
Object** objectsLightVec;
int nbLights = 0;


DEVICE afloat get_random_float()
{
	return curand_uniform(&g_statesDevice[blockIdx.x *blockDim.x + blockIdx.y]);
}

__global__ void setup_curand(curandState_t* states, unsigned long seed)
{
	curand_init(seed, threadIdx.x + blockIdx.x * blockDim.x, 0, &states[threadIdx.x + blockIdx.x * blockDim.x]);
}


__global__ void setup_globals(curandState_t* states, Vector3* bgColour)
{
	g_statesDevice = states;
	g_BgColourDevice = bgColour;


}

unsigned int* AllocateBuffer(unsigned int w, unsigned int h, unsigned int blockSize, const Vector3& bgColour)
{
	width = w;
	height = h;
	unsigned int* p;
	cudaMalloc(&p, w*h*sizeof(int));
	intFrameBuffer = p;
	cudaMalloc(&frameBuffer, w*h*sizeof(Vector3));
	cudaMalloc((void**)&objectsLightVec, (sizeof(Object*)* 1024));

	int N = width * height;
	int numBlocks = (N + blockSize - 1) / blockSize;
	cudaMalloc((void**)&cudaCamera, sizeof(Camera));

	cudaMalloc((void**)&g_BgColour, sizeof(Vector3));
	cudaMemcpy(g_BgColour, &bgColour, sizeof(Vector3), cudaMemcpyHostToDevice);


	cudaMalloc((void**)&g_states, numBlocks * blockSize * sizeof(curandState_t));
	setup_globals << <1, 1 >> >(g_states, g_BgColour);
	cudaDeviceSynchronize();



	return p;

}

DEVICE andro::Vector3 get_color(andro::ray& ray, unsigned int depth, Object** objects, int nbObjects, curandState_t* states, Object** lights, int nLights)
{
	uint16_t nbBounce = depth;
	andro::ray current_ray = ray;
	andro::Vector3 attenuation(1);
	andro::Vector3 final_colour(0.0);

	for (uint16_t bounce = 0; bounce < nbBounce; bounce++)
	{

		afloat closest_so_far = 99999.0f;
		andro::hit_record rec, temp_rec;
		bool hit = false;
		const material* current_mat = nullptr;
		current_ray.dir.NormalizeInto();
		for (int i = 0; i < nbObjects; i++)
		{
			Object* obj = objects[i];
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

			emited = current_mat->emitted(0, 0, rec.point);

			{
				for (int l = 0; l < nLights; l++)
				{

					afloat light_closest_so_far = 99999.0f;
					andro::hit_record lrec, ltemp_rec;
					bool lhit = false;
					Object*  lobject = NULL;


					if (rec.object == lights[l]->m_shape)
					{
						emited = lights[l]->m_material->emitted(0, 0, rec.point);
						continue;
					}

					andro::Vector3 toLightDir = lights[l]->GetBoundingSphere().center - rec.point;
					float inv_rsq = 1.0 / toLightDir.LenghtSq();

					toLightDir = toLightDir + random_in_unit_sphere() * 0.3;
					toLightDir.NormalizeInto();
					andro::ray lightRay(rec.point, toLightDir);


					for (int k = 0; k < nbObjects; k++)
					{
						Object* obj_l = objects[k];
						if (obj_l->m_shape->hit(lightRay, 0.0001f, light_closest_so_far, ltemp_rec))
						{
							lhit = true;
							light_closest_so_far = ltemp_rec.t;
							lrec = ltemp_rec;
							lobject = obj_l;
						}
					}

					if (lhit && lrec.t > 0 && lobject == lights[l])
					{
						float cos = toLightDir * rec.normal;
						emited = emited + lobject->m_material->emitted(0, 0, rec.point) * inv_rsq * fmaxf(cos, 0);
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
			return final_colour + attenuation.Multiply(*g_BgColourDevice);
		}
	return final_colour;

}

// Kernel function to add the elements of two arrays
__global__
void renderFrame(int n, andro::Vector3* frameBuffer, int width, int height, const Camera * pCamera, Object** objects, int nbObjects, curandState_t* states, Object** lights, int nLights)
{

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;

	for (int i = index; i < n; i += stride)
	{

		int x = i % width;
		int y = i / width;

		afloat u = afloat(x + get_random_float() * 2 - 1 ) / width;
		afloat v = afloat(y + get_random_float() * 2 - 1 ) / height;
		andro::ray r = pCamera->getRay(u, v);
		andro::Vector3 color = get_color(r, 10, objects, nbObjects, states, lights, nLights);

		__syncthreads();
		frameBuffer[i] = frameBuffer[i] + color;
	}
}


__global__
void copyFrame(int n, unsigned int* p, andro::Vector3* frameBuffer, int samples)
{

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	for (int i = index; i < n; i += stride)
	{
		Vector3 currentCol = frameBuffer[i] * (1.0f/ samples);
		//gamma correction
		currentCol = andro::Vector3(sqrtf(currentCol.x), sqrtf(currentCol.y), sqrtf(currentCol.z));

		p[i] = int(currentCol.x * 255) << 16;
		p[i] |= int(currentCol.y * 255) << 8;
		p[i] |= int(currentCol.z * 255);

	}
}


int FrameUpdateCuda(const Camera * pCamera, Object** objects, int nbObjects, unsigned long seed, unsigned int nbSamples, unsigned int blockSize)
{
	static int samples = 1;
	
	int N = width * height;
	int numBlocks = (N + blockSize - 1) / blockSize;

	cudaMemcpy(cudaCamera, pCamera, sizeof(Camera), cudaMemcpyHostToDevice);


	setup_curand << <numBlocks, blockSize >> >(g_states, seed);
	cudaDeviceSynchronize();

	renderFrame << <numBlocks, blockSize >> >(N, frameBuffer, width, height, cudaCamera, objects, nbObjects, g_states, objectsLightVec, nbLights);
	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();


	copyFrame << <numBlocks, blockSize >> >(N, intFrameBuffer, frameBuffer, samples++);
	cudaDeviceSynchronize();

	// Check for errors (all values should be 3.0f)
	return 0;
}

void cudaFreeOjects()
{
	cudaFree(g_states);
	cudaFree(cudaCamera);
	cudaFree(objectsLightVec);
	cudaFree(intFrameBuffer);
	cudaFree(frameBuffer);
	cudaFree(g_BgColour);
	

}


__global__ void init_kernel(Object** obj, ObjectDesc description, Object** lightObject)
{
	*obj = CreateFromObjectDesc(description);
	if (description.m_material == MaterialType::M_LIGHT)
		*lightObject = *obj;
}


int CreateCudaObject(Object** object, ObjectDesc& desc)
{
	init_kernel << <1, 1 >> > (object, desc, &objectsLightVec[nbLights]);
	cudaDeviceSynchronize();
	if (desc.m_material == MaterialType::M_LIGHT)
		nbLights++;

	return 0;
}
#endif //_USE_CUDA