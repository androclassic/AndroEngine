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

#include "../../AndroSDK/glm/glm/glm.hpp"

class GameObject
{

public:
	GameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName);
	~GameObject();
	void SetPosition(afloat x, afloat y, afloat z);
	void SetScale(afloat scale);

	static ObjectRef<GameObject> CreateGameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName);
	static bool DestroyGameObject(ObjectRef<GameObject> pObject);

private:
	TakeTwo::Node mNode;
	TakeTwo::Node mOctreeNode;
	static std::vector<GameObject*> m_gameObjects;

};