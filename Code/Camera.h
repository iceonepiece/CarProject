#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	virtual glm::mat4 GetViewProjectionMatrix() const
	{
		return glm::mat4(1.0f);
	}

	glm::vec3 GetPosition() const
	{
		return m_position;
	}

protected:
	glm::vec3 m_position;
};