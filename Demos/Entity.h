#pragma once

#include "../EventManager/EventManager.h"
#include "../Force/core/Force.h"
#include "../AndroRenderer/AndroRenderer.h"

#include <stdio.h>
#include <cassert>

#ifndef	AndroToForce
#define AndroToForce(v) force::Vector3(v.x, v.y, v.z)
#endif

#ifndef	ForceToAndro
#define ForceToAndro(v) andro::Vector3(v.x, v.y, v.z)
#endif


class Entity
{
	public:
		Entity();
		~Entity();

		virtual void Update(float dt);
		force::Primitive* GetPhysicObject() const;
		andro::DynamicObject* GetVisualObject() const;

protected:
		void UpdateVisualObjectTransformMatrix();
		
		shared_ptr<andro::DynamicObject>		m_visualObject;
		shared_ptr<force::Primitive>			m_physicObject;
			
};


