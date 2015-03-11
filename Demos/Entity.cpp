#include "Entity.h"

Entity::Entity() : m_visualObject(NULL),m_physicObject(NULL)
{
}

Entity::~Entity() 
{
	//objects pointers are managed by specific modules
	if(m_visualObject != NULL && andro::Engine::GetInstance() != NULL)
		assert(andro::Engine::GetInstance()->GetScene()->RemoveObject(m_visualObject) == false);
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
		m_visualObject->m_WorldMatrix.CRight = ForceToAndro(v);
		m_visualObject->m_WorldMatrix.Tx = m.data[3];

		force::Vector3 v1(m.data[4],m.data[5],m.data[6]);
		m_visualObject->m_WorldMatrix.CUp = ForceToAndro(v1);
		m_visualObject->m_WorldMatrix.Ty = m.data[7];

		force::Vector3 v2(m.data[8],m.data[9],m.data[10]);
		m_visualObject->m_WorldMatrix.CLook = ForceToAndro(v2);
		m_visualObject->m_WorldMatrix.Tz = m.data[11];

		force::Vector3 pos = m_physicObject->rigidBody->GetPosition();
		m_visualObject->SetPosition(ForceToAndro(pos));
	}
}

void Entity::Update(float dt)
{
	UpdateVisualObjectTransformMatrix();
}
