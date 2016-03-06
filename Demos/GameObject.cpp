#include "GameObject.h"
std::vector<GameObject*> GameObject::m_gameObjects;


GameObject::GameObject(TakeOne::Material::MaterialFormat pMaterialFormat, const char* pModelName)
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

GameObject::~GameObject()
{
	TakeOne::Engine::GetInstance()->RemoveRenderObject(&mRenderNode);
}

void GameObject::SetPosition(float x, float y, float z)
{
	mRenderNode.GetTransform().SetPosition(glm::vec3(x, y, z));
}
void GameObject::SetScale(float scale)
{
	mRenderNode.GetTransform().SetScale(glm::vec3(scale, scale, scale));
}


ObjectRef<GameObject> GameObject::CreateGameObject(TakeOne::Material::MaterialFormat pMaterialFormat, const char* pModelName)
{
	GameObject* new_obj = new GameObject(pMaterialFormat, pModelName);
	m_gameObjects.push_back(new_obj);

	ObjectRef<GameObject> ref;
	ref.object = new_obj;

	return ref;
}

bool GameObject::DestroyGameObject(ObjectRef<GameObject>  pObject)
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if (*it == pObject.object)
		{
			delete pObject.object;
			m_gameObjects.erase(it);
			return true;
		}
	}
	return false;
}

