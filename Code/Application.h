#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

class Application
{
public:
    Application();

    void ProcessInput();
    void SwapBuffers();

    bool WindowShouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    static Application& Get()
    {
        return *s_instance;
    }

    glm::vec2 GetWindowSize();

private:
    static Application* s_instance;

    GLFWwindow* window = nullptr;
};