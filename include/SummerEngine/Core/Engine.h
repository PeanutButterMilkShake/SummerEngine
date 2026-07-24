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

private:
    float lastFrame;
};