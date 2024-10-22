#pragma once

#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Application.h"
#include "Input.h"

class FollowCamera : public Camera
{
public:
    bool isFollowView = true;

    FollowCamera(Car& car)
        : m_targetCar(car)
    {
        m_position = glm::vec3(0, 0, 0);
        m_targetPosition = m_position;
    }

    void Update(float dt)
    {
        if (Input::GetKeyDown(GLFW_KEY_SPACE))
            isFollowView = !isFollowView;

        glm::vec3 carDirection = m_targetCar.m_forward;
        glm::vec3 k = -glm::normalize(m_targetCar.m_forward);

        if (!isFollowView)
            k = glm::normalize(glm::cross({ 0, 1, 0 }, carDirection));

        m_targetPosition = m_targetCar.m_position + (k * m_cameraDistance);
        m_targetPosition.y += m_cameraY;

        glm::vec3 moveDirection = m_targetPosition - m_position;
        m_position = Lerp(m_position, m_targetPosition, dt * m_cameraSpeed);
    }

    virtual glm::mat4 GetViewProjectionMatrix() const
    {
        glm::mat4 view = glm::lookAt(m_position, m_targetCar.m_position, glm::vec3(0, 1, 0));

        glm::vec2 windowSize = Application::Get().GetWindowSize();
        glm::mat4 projection = glm::perspective(glm::radians(m_zoom), (float)windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);

        return projection * view;
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
    float m_zoom = 60.0f;
};
