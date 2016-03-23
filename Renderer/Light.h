#pragma once
#include "glm\glm.hpp"

namespace TakeTwo
{
	class Light
	{
	public:
		glm::vec3 position;
		glm::vec3 intensities;
		float attenuation;
		float ambientCoefficient;
	};
}