#include "Renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


Renderer::Renderer()
    //: m_baseShader("Assets/Shaders/1.model_loading.vs", "Assets/Shaders/1.model_loading.fs")
    : m_baseShader("Assets/Shaders/lighting.vs", "Assets/Shaders/lighting.fs")
    , m_depthShader("Assets/Shaders/shadow_depth.vs", "Assets/Shaders/shadow_depth.fs")
{
    InitializeDepthMap();
}


void Renderer::InitializeDepthMap()
{
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_baseShader.use();
    m_baseShader.setInt("diffuseTexture", 0);
    m_baseShader.setInt("shadowMap", 1);
}

void Renderer::DrawPlane(glm::mat4& model, Shader& shader)
{
;
}

void Renderer::BeginFrame(Camera camera)
{
    m_camera = camera;

    glClearColor(0.007, 0.07, 0.138, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}