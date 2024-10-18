#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <iostream>
#include <vector>

#include "Application.h"
#include "Car.h"
#include "FollowCamera.h"
#include "StaticObject.h"
#include "Audio.h"
#include "Renderer.h"
#include "Input.h"

int main()
{
    Application app;

    //Audio audio;
    //audio.LoadSound("CarEngine", "Assets/Audio/Car.wav");

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("Assets/Shaders/1.model_loading.vs", "Assets/Shaders/1.model_loading.fs");

    Renderer renderer;

    Car playerCar("Assets/Models/car/racer_nowheels.obj", glm::vec3{ 0, 2, 0 }, glm::vec3{ 1.0f });
    FollowCamera followCamera(playerCar);
    
    std::vector<StaticObject> objects{
        { "Assets/Models/moscow/moscow.obj", { 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, glm::vec3{80.0f} }
        //{ "Assets/Models/pirate/port_royale.obj", { 0.0f, -10.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, glm::vec3{40.0f} }
    };

    int frameCount = 0;
    float fpsTime = 0.0f;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // render loop
    // -----------
    while (!app.WindowShouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Count the frames
        frameCount++;
        fpsTime += deltaTime;

        // Calculate FPS and output once per second
        if (fpsTime >= 1.0f) // One second has passed
        {
            //std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;    // Reset the frame count
            fpsTime = 0.0f;    // Reset the timer
        }

        app.ProcessInput();

        playerCar.Update(deltaTime);
        //playerCar.AudioUpdate(audio, deltaTime, window);
        playerCar.CheckCollisions(objects, deltaTime);
        followCamera.Update(deltaTime);

        // render
        // ------
        glClearColor(0.007, 0.07, 0.138, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightPosition{ 0, 40, 0 };

        ourShader.use();
        ourShader.setVec3("lightPosition", lightPosition);
        ourShader.setMat4("viewProjection", followCamera.GetViewProjectionMatrix());

        playerCar.Render(ourShader);

        for (auto& obj : objects)
            obj.Render(ourShader);

        app.SwapBuffers();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
