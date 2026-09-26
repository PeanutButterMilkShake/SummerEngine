#include "SETime.h"

GLFWwindow *Time::window = nullptr;
float Time::deltaTime = 0.0f;
int Time::fps = 0.0f;

float lastFrame = static_cast<float>(glfwGetTime());
void Time::Update()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    static float fpsTimer = 0.0f;
    static int frameCount = 0;

    fpsTimer += deltaTime;
    frameCount++;

    if (fpsTimer >= 1.0f)
    {
        fps = frameCount;
        frameCount = 0;
        fpsTimer = 0.0f;

        std::string title = "Engine | FPS: " + std::to_string(fps);
        glfwSetWindowTitle(window, title.c_str());
    } 
}

float Time::GetTime()
{
    return static_cast<float>(glfwGetTime());
}

double Time::GetTimeDouble()
{
    return glfwGetTime();
}