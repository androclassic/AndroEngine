#pragma once
#include <windows.h>
#include "Window.h"
#include <time.h>
#include <memory.h>
#include <sstream>

#include "Camera.h"

#include "../../AndroUtils/EventManager/Event.h"
#include "../../AndroUtils/EventManager/EventManager.h"

#include "../../TakeOneRenderer/Program.h"
#include "../../TakeOneRenderer/Engine.h"
#include "../../TakeOneRenderer/RenderNode.h"

#include ".././AndroUtils/Introspection/LuaState.h"

#include "../../AndroSDK/glm/glm/glm.hpp"



class Game
{
public:
	Game();

	void Initialise();
	void Update(float dt);
	void End();

	inline Window& GetWindow() { return *mWindow.get(); }

private:
	std::unique_ptr<Window> mWindow;
	std::unique_ptr<Camera> mCamera;
	std::unique_ptr<TakeOne::Light> mMainLight;

};


class GameObject
{

public:
	GameObject(TakeOne::Material::MaterialFormat pMaterialFormat, const char* pModelName)
	{
		std::stringstream resource_key;
		resource_key << pModelName << "_" << pMaterialFormat.programName << "_" << pMaterialFormat.textureName;

		TakeOne::RenderObject::RenderObjectArgs args(pMaterialFormat, pModelName);

		TakeOne::RenderObject *renderObject = TakeOne::Engine::GetInstance()->GetResoruceManager().Load<TakeOne::RenderObject>(resource_key.str(), &args);
		mRenderNode.SetRenderObject(renderObject);
		TakeOne::Engine::GetInstance()->RegisterRenderObject(&mRenderNode);

		mRenderNode.GetTransform().SetScale(glm::vec3(0.1f));
		mRenderNode.GetTransform().SetPosition(glm::vec3(0, 0, 0));

	}

	void SetPosition(float x, float y, float z)
	{
		mRenderNode.GetTransform().SetPosition(glm::vec3(x, y, z));
	}
	void SetScale( float scale)
	{
		mRenderNode.GetTransform().SetScale(glm::vec3(scale, scale, scale));
	}


	static ObjectRef<GameObject> CreateGameObject(TakeOne::Material::MaterialFormat pMaterialFormat, const char* pModelName)
	{
		GameObject* new_obj = new GameObject(pMaterialFormat, pModelName);
		m_gameObjects.push_back(new_obj);

		ObjectRef<GameObject> ref;
		ref.object = new_obj;

		return ref;
	}

private:
	TakeOne::RenderNode mRenderNode;
	static std::vector<GameObject*> m_gameObjects;

};


