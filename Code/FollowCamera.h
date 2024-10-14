#pragma once

class FollowCamera
{
public:
    bool isTopView = false;

    FollowCamera(Car& car)
        : m_targetCar(car)
        , m_position(glm::vec3(0, 0, 0))
        , m_targetPosition(m_position)
    {
    }

    void Update(float dt)
    {
        glm::vec3 carDirection = m_targetCar.m_forward;
        glm::vec3 k = glm::normalize(-carDirection);

        glm::vec3 up = glm::vec3(0, 1, 0); // World up vector (y-axis)
        glm::vec3 right = glm::normalize(glm::cross(up, carDirection));

        if (isTopView)
            k = right;

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
    float m_cameraY = 1.75f;
    float m_cameraDistance = 4.25f;

    float theta = 30.0f;
    Car& m_targetCar;
    glm::vec3 m_targetPosition;
    glm::vec3 m_position;
    float m_zoom = 60.0f;
};
