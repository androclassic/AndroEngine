#pragma once
#include "glm\glm.hpp"

namespace TakeOne
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