#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Utility.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Camera.h"
#include "Input.h"
#include "NetworkManager.h"
#include "EngineUI.h"

class Object;
class Light;

class Engine
{
public:
    GLFWwindow *window;
    static std::vector<Object*> objects;
    static Camera* mainCamera;
    static float delta;
    static int fps;
    static std::vector<Light*> lights;
    static Vector2 windowDimensions;
    static EngineUI* engineUI;

    // TEMP CHANGE WHEN ENGINE UI STUFF
    static GLuint whiteTextureId;

    static void AddObject(Object *object)
    {
        objects.push_back(object);
    }

    Engine(string name);
    ~Engine();

    void Initialize(string name);
    void Update();
    void CalculateTimeData();
    void ResetTime();
    static void ErrorCallback(int error, const char* description);
    static void WindowResizeCallback(GLFWwindow* window, int width, int height);
    static void APIENTRY OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

private:
    float lastFrame;
};