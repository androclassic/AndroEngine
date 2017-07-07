#pragma once

#include "../../EventManager/EventManager.h"
#include "../../Force/core/Force.h"
#include "../../AndroRenderer/AndroRenderer.h"
#include "Entity.h"

#include <stdio.h>
#include <cassert>

#ifndef	AndroToForce
#define AndroToForce(v) force::Vector3(v.x, v.y, v.z)
#endif

#ifndef	ForceToAndro
#define ForceToAndro(v) andro::Vector3(v.x, v.y, v.z)
#endif


class Surface : public Entity
{
	public:
		Surface(float offset , force::Vector3 normal, andro::Vector3 color, andro::Vector2 quadSize);
};

Surface::Surface(float offset , force::Vector3 normal, andro::Vector3 color, andro::Vector2 quadSize) : Entity()
{
	normal.normalise();
	force::Vector3 pos =  normal * offset;
	m_visualObject =  make_shared<andro::Quad>(ForceToAndro(pos),quadSize, color);	
	
	if(normal.x == 1 || normal.x == -1)
	{
		m_visualObject->SetHeading(90);
		m_visualObject->SetPitch(90);
	}

	if(normal.z == 1 || normal.z == -1)
	{
		m_visualObject->SetPitch(90);
	}

	andro::Engine::GetInstance()->GetScene()->AddDynamicObject(m_visualObject);

	m_physicObject = make_shared<force::Plane>();
	
	m_physicObject->rigidBody = new force::RigidBody();
	m_physicObject->rigidBody->SetPosition(normal * offset);
	((force::Plane*)m_physicObject.get())->normal = normal;
	((force::Plane*)m_physicObject.get())->offset = offset;
}

