#include "Scene.h"


#ifdef _USE_CUDA
#include "cuda_device_runtime_api.h"
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>

extern int CreateCudaObject(Object** object, const ObjectDesc& desc);
#endif


CScene::CScene()
{
#ifdef _USE_CUDA
	cudaMalloc((void**)&objectsVec, (sizeof(Object*) * 1024));
#endif

}

CScene::~CScene()
{
	DestroyScene();
#ifdef _USE_CUDA
	cudaFree(objectsVec);
#endif
}


void CScene::AddObject(Object* obj, const ObjectDesc& pDesc)
{

#ifdef _USE_CUDA
	CreateCudaObject(&objectsVec[m_objects.size()], pDesc);
#endif

	m_objects.push_back(obj);
	if (pDesc.m_material == MaterialType::M_LIGHT)
	{
		AddLight(obj);
	}
}
void CScene::AddLight(const Object* obj)
{
	m_lightObjects.push_back(obj);
}

const andro::OctreeNode<Object*>const* CScene::GetOctree() const
{
	return m_octree;
}

std::vector<Object*>::const_iterator CScene::GetObjectsBegin() const
{
	return std::cbegin(m_objects);
}

std::vector<Object*>::const_iterator CScene::GetObjectsEnd() const
{
	return std::cend(m_objects);
}


int CScene::GetObjectsCount() const
{
	return m_objects.size();
}

const std::vector<const Object*>& CScene::GetLights() const
{
	return m_lightObjects;
}

void CScene::DestroyScene()
{
	for (auto obj : m_objects)
	{
		obj->m_material->Destroy();
		delete obj->m_material;
		delete obj;
	}
	delete m_octree;
}




void CScene::InitScene()
{

	// buid scene bbx
	afloat min_radius = 10000;

	int i = 0;
	for (auto object : m_objects)
	{
		andro::Sphere bsphere = object->GetBoundingSphere();

		min_radius = fminf(min_radius, bsphere.radius);
		for (int a = 0; a < 3; a++)
		{

			m_scene_bbx.min[a] = fminf(m_scene_bbx.min[a], bsphere.center[a] - bsphere.radius);
			m_scene_bbx.max[a] = fmaxf(m_scene_bbx.max[a], bsphere.center[a] + bsphere.radius);
		}
	}

	BuildSceneOctree();
}



void CScene::BuildSceneOctree()
{
	// build octree of the scene
	m_octree = andro::BuildOctree<Object*>(m_objects, m_scene_bbx, 21, [](const andro::BoundingBox& box, Object* t)
	{
		// check in sphere is enclosed
		andro::Sphere bsphere = t->GetBoundingSphere();

		andro::Vector3 v = (bsphere.center - box.GetCenter());
		andro::Vector3 halfsize = box.GetHalfSize();

		if ((halfsize.x > std::fabs(v.x) + bsphere.radius) &&
			(halfsize.y > std::fabs(v.y) + bsphere.radius) &&
			(halfsize.z > std::fabs(v.z) + bsphere.radius))
			return true;

		return false;

	}, true);

}