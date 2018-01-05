#include "GameObject.h"
std::vector<GameObject*> GameObject::m_gameObjects;


GameObject::GameObject(TakeTwo::Material::MaterialFormat pMaterialFormat, const char* pModelName, force::Primitive* physicsObj)
	:m_physicObject(physicsObj)
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

	// physic
	if (physicsObj != nullptr)
	{
		//		m_physicObject->rigidBody->orientation = orientation;
		static std::unique_ptr<force::ParticleGravity> gravity_generator = make_unique<force::ParticleGravity>(force::Vector3(0, -9.8, 0));
		force::World::GetInstance()->registry.add(m_physicObject->rigidBody, gravity_generator.get());
		force::World::GetInstance()->AddPrimitive(m_physicObject.get());
	}



}

GameObject::~GameObject()
{
	TakeTwo::Engine::GetInstance()->RemoveRenderObject(&mNode);
}

void GameObject::SetPosition(afloat x, afloat y, afloat z)
{
	mNode.SetPosition(glm::vec3(x, y, z));

	if (m_physicObject.get() != nullptr)
	{
		m_physicObject->rigidBody->SetPosition(force::Vector3(x, y, z));
	}

}
void GameObject::SetScale(afloat scale)
{
	mNode.SetScale(glm::vec3(scale, scale, scale));
}


ObjectRef<GameObject> GameObject::CreateGameObject(const char* pModelName, TakeTwo::Material::MaterialFormat pMaterialFormat, ObjectRef<force::Primitive> primitiveRef)
{
	GameObject* new_obj = new GameObject(pMaterialFormat, pModelName, primitiveRef.object);
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


void GameObject::NativeUpdate()
{
	if (m_physicObject != NULL )
	{
		force::Matrix4 m = m_physicObject->rigidBody->transformMatrix;

		force::Vector3 v(m.data[0], m.data[1], m.data[2]);
		glm::mat4 mat;
		for (unsigned int i = 0; i < 12; i++)
			mat[i/3][i%4] = m.data[i];

		force::Vector3 pos = m_physicObject->rigidBody->GetPosition();
		mNode.SetRotation(glm::quat_cast(mat));
		mNode.SetPosition(glm::vec3(pos.x, pos.y, pos.z));
	}
}
