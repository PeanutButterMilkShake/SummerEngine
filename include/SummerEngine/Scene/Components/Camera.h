#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Object.h"

class Engine;

class Camera : public Component
{
public:
    Transform* transform;

    float fov = 50;
    float nearPlane = 0.01;
    float farPlane = 1000;

    void Start() override;

    glm::mat4 ViewMatrix();
    glm::mat4 PerspectiveMatrix();
};