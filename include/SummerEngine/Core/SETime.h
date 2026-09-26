#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "MathTypes.h"

namespace Time
{
    extern GLFWwindow *window;

    extern float deltaTime;
    extern int fps;

    void Update();
    float GetTime();
    double GetTimeDouble();
}