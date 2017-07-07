#pragma once

#include "../../../Force/core/Force.h"
#include "../../../AndroRenderer/AndroRenderer.h"


//initialize physics


class Rope  
{	
public:
	Rope(andro::Vector3 position,int elements,float size, float mass);
	void Update(float dt);
	void AddForce(force::Vector3 force, unsigned int index);
	force::Particle* GetFirstElement();
	force::Particle* GetLastElement();
private:

	shared_ptr<andro::DynamicObject>	 mesh[100];
	shared_ptr<andro::Line>				line[100];
	shared_ptr<force::Particle>			 m_particle[100];
	int m_elements;

};