#pragma once
#include <windows.h>
#include <time.h>
#include <memory.h>
#include <sstream>

#include "Window.h"
#include "Camera.h"

#include "../../AndroUtils/EventManager/Event.h"
#include "../../AndroUtils/EventManager/EventManager.h"

#include "../../TakeOneRenderer/Program.h"
#include "../../TakeOneRenderer/Engine.h"
#include "../../TakeOneRenderer/RenderNode.h"

#include ".././AndroUtils/Introspection/LuaState.h"

#include "../../AndroSDK/glm/glm/glm.hpp"

class GameObject
{

public:
	GameObject(TakeOne::Material::MaterialFormat pMaterialFormat, const char* pModelName);
	~GameObject();
	void SetPosition(float x, float y, float z);
	void SetScale(float scale);

	static ObjectRef<GameObject> CreateGameObject(TakeOne::Material::MaterialFormat pMaterialFormat, const char* pModelName);
	static bool DestroyGameObject(ObjectRef<GameObject> pObject);

private:
	TakeOne::RenderNode mRenderNode;
	static std::vector<GameObject*> m_gameObjects;

};