#pragma once
#include <windows.h>
#include <time.h>
#include <memory.h>
#include <sstream>

#include "Window.h"
#include "Camera.h"

#include "../../AndroUtils/EventManager/Event.h"
#include "../../AndroUtils/EventManager/EventManager.h"

#include "../../Renderer/Program.h"
#include "../../Renderer/Engine.h"
#include "../../Renderer/RenderNode.h"

#include ".././AndroUtils/Introspection/LuaState.h"

#include "../../AndroSDK/glm/glm/glm.hpp"

class GameObject
{

public:
	GameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName);
	~GameObject();
	void SetPosition(float x, float y, float z);
	void SetScale(float scale);

	static ObjectRef<GameObject> CreateGameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName);
	static bool DestroyGameObject(ObjectRef<GameObject> pObject);

private:
	TakeTwo::RenderNode mRenderNode;
	static std::vector<GameObject*> m_gameObjects;

};