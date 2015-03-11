#pragma once

#include "../../EventManager/EventManager.h"
#include "../../Force/core/Force.h"
#include "../../AndroRenderer/AndroRenderer.h"

#define ELEMENTS 10

class Entity  : public andro::EventListener
{	
public:
	Entity(andro::Vector3 position,float size, int mass);
	void Update(float dt);
	void AddForce(force::Vector3 force, unsigned int index);
	void OnEvent(andro::Event* e);

private:

	shared_ptr<andro::DynamicObject> mesh[ELEMENTS];
	shared_ptr<andro::Line>			 line[ELEMENTS];
	force::Particle m_particle[ELEMENTS];
	int sensX;
	int sensY;
	int sensZ;
	float timer1;
	float timer2;
	float timer3;


};

