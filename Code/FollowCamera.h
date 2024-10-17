#pragma once

#include <GLFW/glfw3.h>

class FollowCamera
{
public:
    bool isFollowView = true;

    FollowCamera(Car& car)
        : m_targetCar(car)
        , m_position(glm::vec3(0, 0, 0))
        , m_targetPosition(m_position)
    {
    }

    void Update(GLFWwindow* window, float dt)
    {
        glm::vec3 carDirection = m_targetCar.m_forward;
        glm::vec3 k = -glm::normalize(m_targetCar.m_forward);

        if (!isFollowView)
            k = glm::normalize(glm::cross({ 0, 1, 0 }, carDirection));

        m_targetPosition = m_targetCar.m_position + (k * m_cameraDistance);
        m_targetPosition.y += m_cameraY;

        glm::vec3 moveDirection = m_targetPosition - m_position;
        m_position = Lerp(m_position, m_targetPosition, dt * m_cameraSpeed);
    }

    glm::mat4 GetViewMatrix() const
    {
        glm::vec3 target = m_targetCar.m_position;
        return glm::lookAt(m_position, target, glm::vec3(0, 1, 0));
    }

    float GetZoom() const
    {
        return m_zoom;
    }

private:
    float m_cameraSpeed = 15.0f;
    float m_delayTimer = 0.0f;
    float m_cameraY = 2.0f;
    float m_cameraDistance = 4.25f;

    float theta = 30.0f;
    Car& m_targetCar;
    glm::vec3 m_targetPosition;
    glm::vec3 m_position;
    float m_zoom = 60.0f;
};
