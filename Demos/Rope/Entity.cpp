#include "Entity.h"


Entity::Entity(andro::Vector3 position,float size, int mass)
{
	REGISTER_LISTENER('MV',this);
	sensY = sensX = sensZ = 1;
	timer1 = timer2 = timer3 = 0;

	assert(andro::Engine::GetInstance() != NULL);

	force::ParticleWorld& physicsEngine = *force::ParticleWorld::GetInstance();



	for(int i=0; i < ELEMENTS; i++)
	{
		mesh[i] = make_shared<andro::Cube>(andro::Vector3(position.x  - size * i * 5, position.y,position.z) ,andro::Vector3(size,size,size),andro::Vector3(0,4,0));
		mesh[i]->SetPitchSpeed(50.0f);
		andro::Engine::GetInstance()->GetScene()->AddDynamicObject(mesh[i]);


		m_particle[i].SetMass(mass);

		m_particle[i].SetPosition(force::Vector3(position.x - size * i * 5, position.y, position.z));

		m_particle[i].SetDamping(0.1f);
		physicsEngine.registry.add(&m_particle[i],new force::ParticleGravity());
		physicsEngine.AddParticles(&m_particle[i], 1);


	}
		m_particle[0].SetMass(mass*6);
		physicsEngine.AddParticleContactGenerators(new force::GroundContacts(-5),1);
	
	for(int i=0;i < ELEMENTS - 1; i++)
	{
			line[i] = make_shared<andro::Line>
			(andro::Vector3(position.x  - size * i * 5, position.y,position.z),
			 andro::Vector3(position.x  - size * (i+1) * 5, position.y ,position.z),0.5);
			andro::Engine::GetInstance()->GetScene()->AddDynamicObject(line[i]);

		physicsEngine.AddParticleContactGenerators(new force::ParticleCable(&m_particle[i], &m_particle[i+1],size * 7),1);

	}


}
void Entity::Update(float dt)
{
	timer1+=dt;
	timer2+=dt;
	timer3+=dt;

	if(timer1>1.0f)
	{
		timer1=0;
		sensY*=-1;
	}
	if(timer2>2.0f)
	{
		timer2=0;
		sensX*=-1;
	}
	if(timer3>15.0f)
	{
		timer3=0;
		sensZ*=-1;
	}

	for(int i=0; i < ELEMENTS; i++)
	{
		force::Vector3 position=m_particle[i].GetPosition();
		mesh[i]->SetPosition(andro::Vector3(position.x, position.y, position.z));
	}
		for(int i=0; i < ELEMENTS -1; i++)
		{
			force::Vector3 position=m_particle[i].GetPosition();
			force::Vector3 orientation=m_particle[i+1].GetPosition();
			line[i]->UpdateLine(andro::Vector3(position.x, position.y, position.z),
								andro::Vector3(orientation.x, orientation.y, orientation.z));
		}
	
	m_particle[0].AddForce(force::Vector3(sensX * 100,sensY* 370,-200 * sensZ));
}


void Entity::AddForce(force::Vector3 force, unsigned int index)
{
	m_particle[index].AddForce(force);
}

void Entity::OnEvent(andro::Event* e)
{
	if(e->GetID()=='MV')
		sensX*=0;
}
