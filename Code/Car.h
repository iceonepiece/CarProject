#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>

#include "Math.h"
#include "StaticObject.h"
const glm::mat4 mat4one = glm::mat4(1.0f);
float KeyframeScale = 0.1;

struct Keyframe {
    float time;       
    glm::vec3 position;
    glm::vec3 scale;  
    glm::vec3 rotation;  

    Keyframe(float t, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
        : time(t), position(pos),rotation(rot), scale(scl) {}
};



class CarGhost;
vector <CarGhost*> All_Ghost;
 
class CarGhost  {
public:
    CarGhost(Model _model,vector<Keyframe> kf)
        : m_model(_model), currentTime(0.0f), currentKeyframeIndex(0)
    {
        keyframes = kf;
        m_position = keyframes[0].position;
        m_rotation = keyframes[0].rotation;
        m_scale = keyframes[0].scale;
        All_Ghost.push_back(this);
    }

    void Update(GLFWwindow* window, float dt); 
     

    Model m_model;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_forward = glm::vec3( 0.0f, 0.0f, 1.0f );


    void Render(Shader& shader)
    {
        glm::mat4 modelRotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0, 1.0f, 0));

        glm::vec3 pos = m_position;
        pos.y -= 0.5f;

        glm::mat4 rotation = glm::mat4(1.0f);

        // Apply rotations in Yaw -> Pitch -> Roll order
        rotation = glm::rotate(rotation, GetYawFromForward(m_forward), glm::vec3(0, 1, 0));   // Yaw (Y-axis)
        rotation = glm::rotate(rotation, m_rotation.x, glm::vec3(1, 0, 0)); // Pitch (X-axis)
        rotation = glm::rotate(rotation, m_rotation.z, glm::vec3(0, 0, 1));  // Roll (Z-axis)

        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos)
            * rotation
            * modelRotation
            * glm::scale(glm::mat4(1.0f), m_scale);

        shader.setMat4("model", model);
        m_model.Draw(shader);


         
    }

private:
    std::vector<Keyframe> keyframes; // Reference to the keyframes
    float currentTime;                // Tracks the elapsed time for interpolation
    bool isPlaying = false;                   // Play/pause state
    int currentKeyframeIndex;         // Index of the current keyframe
};

float Lerp(float start, float end, float t) {
    return start + t * (end - start);
}

glm::vec3 LerpVec3(glm::vec3 start, glm::vec3 end, float t) {
    return glm::vec3(Lerp(start.x, end.x, t),
        Lerp(start.y, end.y, t),
        Lerp(start.z, end.z, t));
}



glm::vec3 TargetPos = glm::vec3(0);
void CarGhost::Update(GLFWwindow* window, float dt) {

    //float steerAngle = steerInput * m_steerFactor;

    //glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(steerAngle), glm::vec3(0, 1, 0));
    //glm::vec3 newForward = rot * glm::vec4{ m_forward.x, m_forward.y, m_forward.z, 0.0f };

    //float amount = dt * glm::exp(m_steerLerpFactor * m_steerPowerCounter);
    //m_forward = glm::normalize(Lerp(m_forward, newForward, amount));


    // Handle input for restart and play/pause
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        currentTime = 0.0f;          // Reset the timer
        currentKeyframeIndex = 0;    // Start at the first keyframe
        isPlaying = true;
    }

    // If paused, exit early
    if (!isPlaying) return;

    // Check if there are keyframes to process
    if (keyframes.empty() || currentKeyframeIndex >= keyframes.size() - 1) {
        return;
    }

    currentTime += dt;

    if (currentKeyframeIndex > keyframes.size()-1) { currentKeyframeIndex = 0; isPlaying = false; }
    Keyframe currentKeyframe = keyframes[currentKeyframeIndex];
    Keyframe nextKeyframe = keyframes[currentKeyframeIndex + 1];
     
    m_position = LerpVec3(m_position, TargetPos, dt * 4);

    if (currentTime > KeyframeScale) {
        TargetPos = nextKeyframe.position;
        m_scale = nextKeyframe.scale;
        currentKeyframeIndex++;
        currentTime = 0;
    }
}






