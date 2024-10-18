#pragma once

#include <glm/glm.hpp>

enum class LightType
{
	None,
	Directional,
	Point,
	Spot,
};

struct Light
{
	LightType type = LightType::None;
	glm::vec3 position{ 0 };
};