#pragma once

#include "../EventManager/EventManager.h"
#include "../Force/Core/Force.h"
#include "../AndroRenderer/AndroRenderer.h"
#include "Entity.h"

#include <stdio.h>
#include <cassert>




class Box : public Entity
{
	public:
		Box(force::Vector3 halfsize ,force::Vector3 pos, bool initialState, bool colored = true , force::Quaternion orientation=force::Quaternion(),float mass=1);
		virtual void Update(float dt);
	private:
		bool lastState;
		bool colored;
};

