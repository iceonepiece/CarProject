#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	virtual glm::mat4 GetViewProjectionMatrix() const
	{
		return glm::mat4(1.0f);
	}
};