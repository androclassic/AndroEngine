#include "Sphere.h"

Sphere::Sphere(float radius ,force::Vector3 pos,bool colored, force::Quaternion orientation,float mass) : Entity()
{
	this->colored = colored;
	m_visualObject = make_shared<andro::DynamicObject>(ForceToAndro(pos), 1);
	m_visualObject->loadOBJ("data/sphere.obj");
	m_visualObject->SetScale(radius, radius, radius);
	if(colored)
		m_visualObject->SetShapeColor(andro::Vector3(0.0f,0.3f,0));

	andro::Engine::GetInstance()->GetScene()->AddDynamicObject(m_visualObject);

	m_physicObject = make_shared<force::Sphere>(radius);

	m_physicObject->rigidBody = new force::RigidBody();
	m_physicObject->rigidBody->SetPosition(pos);
	m_physicObject->rigidBody->orientation=orientation;
	m_physicObject->rigidBody->SetMass((double)mass);
	lastState = true;

}

void Sphere::Update(float dt)
{
	Entity::Update(dt);
	if(colored)
	{
			andro::Vector3 pos = m_visualObject->GetPosition();
		if(!m_physicObject->rigidBody->GetAwake() && lastState)
			{
			m_visualObject->SetShapeColor(andro::Vector3(0.0f,0,0.3f));
			}
		else if(m_physicObject->rigidBody->GetAwake() && !lastState)
			{
				m_visualObject->SetShapeColor(andro::Vector3(0.0f,0.3f,0));
			}
		lastState = m_physicObject->rigidBody->GetAwake();
	}
}