#include "GameObject.h"
std::vector<GameObject*> GameObject::m_gameObjects;


GameObject::GameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName)
{
	std::stringstream resource_key;
	resource_key << pModelName << "_" << pMaterialFormat.programName << "_" << pMaterialFormat.textureName;

	TakeTwo::RenderObject::RenderObjectArgs args;
	args.pMaterialFormat = &pMaterialFormat;
	args.pModelName = pModelName;
	args.pOctree = nullptr;

	TakeTwo::RenderObject *renderObject = TakeTwo::Engine::GetInstance()->GetResoruceManager().Load<TakeTwo::RenderObject>(resource_key.str(), &args);
	mNode.SetRenderObject(renderObject);
	renderObject->SetFlags(ITEM_OPAQUE); //todo
	TakeTwo::Engine::GetInstance()->RegisterRenderObject(&mNode);

	mNode.SetScale(glm::vec3(0.1f));
	mNode.SetPosition(glm::vec3(0, 0, 0));


	args.pMaterialFormat = &pMaterialFormat;
	args.pModelName = nullptr;
	args.pOctree = renderObject->m_octree;
	resource_key << "_octree";

	TakeTwo::RenderObject *renderObjectOctree = TakeTwo::Engine::GetInstance()->GetResoruceManager().Load<TakeTwo::RenderObject>(resource_key.str(), &args);
	mOctreeNode.SetRenderObject(renderObjectOctree);
	renderObjectOctree->SetFlags(ITEM_POLYGON); //todo
	mNode.AddChild(&mOctreeNode);


}

GameObject::~GameObject()
{
	TakeTwo::Engine::GetInstance()->RemoveRenderObject(&mNode);
}

void GameObject::SetPosition(float x, float y, float z)
{
	mNode.SetPosition(glm::vec3(x, y, z));
}
void GameObject::SetScale(float scale)
{
	mNode.SetScale(glm::vec3(scale, scale, scale));
}


ObjectRef<GameObject> GameObject::CreateGameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName)
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

