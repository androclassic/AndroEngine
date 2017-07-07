#pragma once

#include "../EventManager/EventManager.h"
#include "../Force/Core/Force.h"
#include "../AndroRenderer/AndroRenderer.h"
#include "Entity.h"

#include <stdio.h>
#include <cassert>



class Sphere : public Entity
{
	public:
		Sphere(float radius ,force::Vector3 pos,bool colored = true, force::Quaternion orientation=force::Quaternion(),float mass=1);
		virtual void Update(float dt);
	private:
		bool lastState;
		bool colored;

};
