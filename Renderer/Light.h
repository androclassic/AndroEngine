#pragma once
#include "glm\glm.hpp"
#include "../AndroUtils/Utils/AndroTypes.h"

namespace TakeTwo
{
	class Light
	{
	public:
		glm::vec3 position;
		glm::vec3 intensities;
		afloat attenuation;
		afloat ambientCoefficient;
	};
}