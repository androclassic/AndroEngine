#include "Entity.h"

Entity::Entity() : m_visualObject(NULL),m_physicObject(NULL)
{
}

Entity::~Entity() 
{
	//objects pointers are managed by specific modules
	if(m_visualObject != NULL && andro::Engine::GetInstance() != NULL)
		VERIFY(andro::Engine::GetInstance()->GetScene()->RemoveObject(m_visualObject));
}

force::Primitive* Entity::GetPhysicObject() const
{
	return m_physicObject.get();
}
andro::DynamicObject* Entity::GetVisualObject() const
{
	return m_visualObject.get();
}

void Entity::UpdateVisualObjectTransformMatrix()
{
	if ( m_physicObject != NULL && m_visualObject != NULL)
	{
		force::Matrix4 m =  m_physicObject->rigidBody->transformMatrix;
  
		force::Vector3 v(m.data[0],m.data[1],m.data[2]);

		for (bssU32 i = 0; i < 12; i++)
			m_visualObject->m_WorldMatrix.data[i] = m.data[i];

		force::Vector3 pos = m_physicObject->rigidBody->GetPosition();
		m_visualObject->SetPosition(ForceToAndro(pos));
	}
}

void Entity::Update(float dt)
{
	UpdateVisualObjectTransformMatrix();
}
