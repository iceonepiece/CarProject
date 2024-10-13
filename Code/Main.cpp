#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrklang/irrKlang.h>
using namespace irrklang;

//#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <vector>

#include "Car.h"
#include "FollowCamera.h"
#include "StaticObject.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool isTopView = false;


void Print(const glm::vec3& v, std::string header = "")
{
    std::cout << header << v.x << ", " << v.y << ", " << v.z << std::endl;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    ISoundEngine* engine = createIrrKlangDevice();

    if (!engine)
        return 0; // error starting up the engine

    //engine->play2D(FileSystem::getPath("resources/audio/engine-6000.mp3").c_str(), false);

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    //stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("Assets/Shaders/1.model_loading.vs", "Assets/Shaders/1.model_loading.fs");

    // load models
    // -----------
    //RacingTrack racingTrack;
    Car playerCar("Assets/Models/car/racer.obj", glm::vec3{ 0, 2, 0 }, glm::vec3{ 1.0f });
    FollowCamera followCamera(playerCar);

    
    std::vector<StaticObject> objects{
       { "Assets/Models/moscow/moscow.obj", { 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f}, glm::vec3{80.0f} }
    };
    

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int frameCount = 0;
    float fpsTime = 0.0f;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
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
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;    // Reset the frame count
            fpsTime = 0.0f;    // Reset the timer
        }

        // input
        // -----
        processInput(window);

        playerCar.Update(window, deltaTime);

        playerCar.CheckCollisions(objects, deltaTime);

        followCamera.Update(deltaTime);

        // render
        // ------
        glClearColor(0.007, 0.07, 0.138, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();


        glm::vec3 lightPosition = playerCar.GetPosition();
        lightPosition.y += 6;
        lightPosition.z -= 2;

        ourShader.setVec3("lightPosition", lightPosition);

        if (true)
        {
            glm::mat4 projection = glm::perspective(glm::radians(followCamera.GetZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            glm::mat4 view = followCamera.GetViewMatrix();
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);
        }
        else
        {
            glm::vec3 carPosition = playerCar.GetPosition();
            carPosition.z += 2.0f;

            glm::vec3 cameraPosition = carPosition;
            cameraPosition.y += 35.0f;
            cameraPosition.z -= 2.0f;

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            glm::mat4 view = glm::lookAt(cameraPosition, carPosition, glm::vec3(0, 1, 0));
            ourShader.setMat4("projection", projection);
            ourShader.setMat4("view", view);
        }

        //racingTrack.Render(ourShader);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        playerCar.Render(ourShader);

        for (auto& obj : objects)
            obj.Render(ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (!isTopView)
    {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_SPACE)
            isTopView = !isTopView;

    }
}
