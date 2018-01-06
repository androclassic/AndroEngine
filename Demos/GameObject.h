#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include <time.h>
#include <memory.h>
#include <sstream>

#include "Window.h"
#include "Camera.h"

#include "../../AndroUtils/EventManager/Event.h"
#include "../../AndroUtils/EventManager/EventManager.h"

#include "../../Renderer/Program.h"
#include "../../Renderer/Engine.h"
#include "../../Renderer/Node.h"

#include ".././AndroUtils/Introspection/LuaState.h"
#include "../Force/core/Force.h"

#include "../../AndroSDK/glm/glm/glm.hpp"
#include "GameBindings.h"

#ifndef vecToForce3
#define vecToForce3(v) force::Vector3(v.x, v.y, v.z)
#endif
#ifndef vecToglm
#define vecToglm(v) glm::vec3(v.x, v.y, v.z)
#endif


using namespace GameBindings;

class GameObject
{

public:
	GameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName, PrimitiveDesc pPrimitiveFormat);
	~GameObject();
	void SetPosition(afloat x, afloat y, afloat z);
	void SetScale(afloat x, afloat y, afloat z);

	static ObjectRef<GameObject> CreateGameObject(const char* pModelName, TakeTwo::Material::MaterialFormat pMaterialFormat, PrimitiveDesc pPrimitiveFormat);
	static bool DestroyGameObject(ObjectRef<GameObject> pObject);
	const force::Primitive* GetPhysicObject() const {return  m_physicObject.get();  }

	void NativeUpdate();

private:
	TakeTwo::Node mNode;
	TakeTwo::Node mOctreeNode;
	std::unique_ptr<force::Primitive> m_physicObject;
	static std::vector<GameObject*> m_gameObjects;


};