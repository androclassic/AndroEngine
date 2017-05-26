#pragma once

#include <vector>
#include<algorithm>
#include "Camera.h"
#include "RayObjects.h"
#include "AndroUtils/Utils/Singleton.h"
#include "AndroUtils/Utils/ThreadPool.h"
#include "ObjectsLuaDesc.h"



class CScene
{
public:
	SINGLETON(CScene);

	void InitScene();
	void DestroyScene();
	void AddObject(Object* obj, const ObjectDesc& pDesc);
	void AddLight(const Object* obj);

	int GetObjectsCount() const;

	const andro::OctreeNode<Object*>const* GetOctree() const;

	std::vector<Object*>::const_iterator GetObjectsBegin() const;
	std::vector<Object*>::const_iterator GetObjectsEnd() const;

	const std::vector<const Object*>& GetLights() const;


#ifdef _USE_CUDA
	Object** objectsVec; //cuda memory for objects
#endif

private:
	void BuildSceneOctree();


private:
	std::vector<const Object*> m_lightObjects;
	andro::OctreeNode<Object*>* m_octree;
	std::vector<Object*> m_objects;
	andro::BoundingBox m_scene_bbx;

};
