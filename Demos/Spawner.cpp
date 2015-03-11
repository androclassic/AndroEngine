#include "Spawner.h"

Spawner::~Spawner()
{
	for (int i = 0; i < m_count; i++)
	{
		if(m_objects[i] != NULL)
		{
			delete m_objects[i];
			m_objects[i] = NULL;
		}
	}
	delete[] m_objects;

	if(m_TemporaryObject != NULL)
	{
		delete m_TemporaryObject;
		m_TemporaryObject = NULL;
	}

}
Spawner::Spawner(const andro::Camera* camera, int maxObjects)
{
	REGISTER_LISTENER(KeyPressedEvent::ID(),this);
	REGISTER_LISTENER(MouseMove::ID(),this);
	REGISTER_LISTENER(MouseLButtonUp::ID(),this);
	REGISTER_LISTENER(MouseLButtonPressed::ID(),this);

	m_mousemove=false;

	m_TemporaryObject = NULL;
	m_objects = new Entity* [maxObjects];
	m_count = 0;
	m_camera =  camera;
	m_maxCount = maxObjects;
}

void Spawner::OnEvent(andro::Event *e)
{
	if(e->GetID() == KeyPressedEvent::ID() )// 0<- event blocked 
	{
		KeyPressedEvent* event = static_cast<KeyPressedEvent*>(e);
		force::Vector3 pos = (AndroToForce(m_camera->GetPosition()) * 1 ) + ( AndroToForce(m_camera->GetOrientation()) *  10);
		switch(event->GetKey())
		{


			case 'A':
				if(m_count < m_maxCount)
					{
						if(m_TemporaryObject != NULL)
							{
								delete m_TemporaryObject;	
								m_TemporaryObject = NULL;
							}

						m_TemporaryObject = new Box(force::Vector3(1,1,1), pos,true,false,force::Quaternion(),1 );
//						((andro::Cube*)m_TemporaryObject->GetVisualObject())->LoadTexture("data/boxtexture.tga");
			
						m_TemporaryShape = e_Box;
					}
				break;
			case 'D':
				if( m_count < m_maxCount)
					{
						if(m_TemporaryObject != NULL)
						{
							delete m_TemporaryObject;	
							m_TemporaryObject = NULL;
						}

						m_TemporaryObject = new Sphere(1, pos ,false,force::Quaternion(),1 );
						m_TemporaryShape = e_Sphere;
					}
				break;
			case 'S':
				if(m_TemporaryObject != NULL)
				{
					m_TemporaryObject->GetPhysicObject()->rigidBody->AddForce( AndroToForce(m_camera->GetOrientation()) * 5000 );

					force::World::GetInstance()->registry.add(m_TemporaryObject->GetPhysicObject()->rigidBody, new force::ParticleGravity(force::Vector3(0, -98, 0)));
					if(m_TemporaryShape == e_Box)
					{
						force::World::GetInstance()->AddBox((force::Box*)m_TemporaryObject->GetPhysicObject());
					}
					else
						force::World::GetInstance()->AddSphere((force::Sphere*)m_TemporaryObject->GetPhysicObject());

					m_objects[m_count++] = m_TemporaryObject;
					m_TemporaryObject = NULL;
				}
				break;

			}
	}


}

void Spawner::Update(float dt)
{

	if(m_TemporaryObject != NULL)
	{
		force::Vector3 pos = (AndroToForce(m_camera->GetPosition())  ) + ( AndroToForce(m_camera->GetOrientation()) *  10);
		m_TemporaryObject->GetPhysicObject()->rigidBody->SetPosition(pos);
		m_TemporaryObject->Update(dt);
	}

	for (int i = 0; i < m_count; i++)
	{
		m_objects[i]->Update(dt);
	}
}
