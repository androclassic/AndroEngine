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

DEVICE andro::Vector3 get_color(andro::ray& ray, unsigned int depth, Object** objects, int nbObjects, curandState_t* states)
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
			andro::Vector3  emited = current_mat->emitted(0, 0, rec.point);
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
void renderFrame(int n, unsigned int *p, int width, int height, const Camera * pCamera, Object** objects, int nbObjects, curandState_t* states,unsigned int nbSamples)
{

	int index = blockIdx.x * blockDim.x + threadIdx.x;
	int stride = blockDim.x * gridDim.x;
	unsigned int ns = nbSamples;

	for (int i = index; i < n; i += stride)
	{

		andro::Vector3 color;
		int x = i % width;
		int y = i / width;

		for (unsigned int s = 0; s < ns; s++)
		{

			afloat u = afloat(x + get_random_float() * 2 - 1 ) / width;
			afloat v = afloat(y + get_random_float() * 2 - 1 ) / height;
			andro::ray r = pCamera->getRay(u, v);
			andro::Vector3 col = get_color(r, 10, objects, nbObjects, states);
			color = color + col;
		}
		color = color * (1.0f / ns);

		//gamma correction
		color = andro::Vector3(sqrtf(color.x), sqrtf(color.y), sqrtf(color.z));

		__syncthreads();
		p[i] = int(color.x * 255) << 16;
		p[i] |= int(color.y * 255) << 8;
		p[i] |= int(color.z * 255);

	}
}



int FrameUpdateCuda(unsigned int *p, const Camera * pCamera, Object** objects, int nbObjects,  unsigned long seed, unsigned int nbSamples, unsigned int blockSize)
{

	
	int N = width * height;
	int numBlocks = (N + blockSize - 1) / blockSize;

	cudaMemcpy(cudaCamera, pCamera, sizeof(Camera), cudaMemcpyHostToDevice);


	setup_curand << <numBlocks, blockSize >> >(g_states, seed);
	cudaDeviceSynchronize();

	renderFrame << <numBlocks, blockSize >> >(N, p, width, height, cudaCamera, objects, nbObjects, g_states, nbSamples);
	// Wait for GPU to finish before accessing on host
	cudaDeviceSynchronize();

	// Check for errors (all values should be 3.0f)
	return 0;
}

void cudaFreeOjects()
{
	cudaFree(g_states);
	cudaFree(cudaCamera);
}


__global__ void init_kernel(Object** obj, ObjectDesc description)
{
	*obj = CreateFromObjectDesc(description);
}


int CreateCudaObject(Object** object, ObjectDesc& desc)
{
	init_kernel << <1, 1 >> > (object, desc);
	cudaDeviceSynchronize();
	return 0;
}
#endif //_USE_CUDA