#include "Box.h"


Box::Box(force::Vector3 halfsize ,force::Vector3 pos,bool initialState,bool colored ,force::Quaternion orientation,float mass) : Entity()
{

	this->colored = colored;

	m_visualObject = make_shared<andro::DynamicObject>(ForceToAndro(pos), 1);
	m_visualObject->loadOBJ("data/cube.obj");
	m_visualObject->SetScale(halfsize.x,halfsize.y,halfsize.z);
	if(colored)
		m_visualObject->SetShapeColor(andro::Vector3(0.0f,0.3f,0));

	andro::Engine::GetInstance()->GetScene()->AddDynamicObject(m_visualObject);

	m_physicObject =  make_shared<force::Box>( halfsize );

	m_physicObject->rigidBody = new force::RigidBody();
	m_physicObject->rigidBody->SetPosition(pos);
	m_physicObject->rigidBody->orientation=orientation;
	m_physicObject->rigidBody->SetMass((double)mass);
	m_physicObject->rigidBody->SetAwake(initialState);
	lastState = true;
}

void Box::Update(float dt)
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
