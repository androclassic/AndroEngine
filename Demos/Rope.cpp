#include "Rope.h"


Rope::Rope(andro::Vector3 position,int elements,float size, float mass)
{
	assert(andro::Engine::GetInstance() != NULL);
	m_elements = elements;

	for(int i=0; i < elements; i++)
	{
		m_particle[i] = make_shared<force::Particle>();
		m_particle[i]->SetMass(mass);

		m_particle[i]->SetPosition(force::Vector3(position.x  , position.y, position.z + size * i));

		force::World::GetInstance()->registry.add(m_particle[i].get(),new force::ParticleGravity(force::Vector3(0,-9.8,0)));
		force::World::GetInstance()->AddParticles(m_particle[i].get(), 1);

	}
	
	for(int i=0;i < elements - 1; i++)
	{
			line[i] = make_shared<andro::Line>(	andro::Vector3(position.x  - size * i, position.y,position.z),
												andro::Vector3(position.x  - size * (i+1) , position.y ,position.z),0.5);

			andro::Engine::GetInstance()->GetScene()->AddDynamicObject(line[i]);

			force::World::GetInstance()->AddParticleContactGenerators(new force::ParticleCable(m_particle[i].get(), m_particle[i + 1].get(), size + 2), 1);
	}
}
void Rope::Update(float dt)
{
	for(int i=0; i < m_elements; i++)
	{
		force::Vector3 position=m_particle[i]->GetPosition();
	}
		for(int i=0; i < m_elements -1; i++)
		{
			force::Vector3 position=m_particle[i]->GetPosition();
			force::Vector3 orientation=m_particle[i+1]->GetPosition();
			line[i]->UpdateLine(andro::Vector3(position.x, position.y, position.z),
								andro::Vector3(orientation.x, orientation.y, orientation.z));
		}
			

}


force::Particle* Rope::GetLastElement()
{
	return  m_particle[m_elements-1].get();
}

force::Particle* Rope::GetFirstElement()
{
	return  m_particle[0].get();
}

void Rope::AddForce(force::Vector3 force, unsigned int index)
{
	if ( m_particle[index] )
		m_particle[index]->AddForce(force);
}
