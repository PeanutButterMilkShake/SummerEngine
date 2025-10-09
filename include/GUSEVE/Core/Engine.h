#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Utility.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Mesh.h"
#include "Camera.h"
#include "Input.h"

class Object;

class Engine
{
public:
    GLFWwindow *window;
    static std::vector<Object*> objects;
    static Camera* mainCamera;

    static void AddObject(Object *object)
    {
        objects.push_back(object);
    }

    Engine(string name);
    ~Engine();

    void Initialize(string name);
    void Update();

private:

};

#endif