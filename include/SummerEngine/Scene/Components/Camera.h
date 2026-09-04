#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Object.h"
#include "Camera.h.generated.h"
class Engine;

class Camera : public Component
{
public:
    Transform* transform;

    SPROPERTY();
    float fov = 50;
    SPROPERTY();
    float nearPlane = 0.01;
    SPROPERTY();
    float farPlane = 1000;

    void Start() override;

    glm::mat4 ViewMatrix();
    glm::mat4 PerspectiveMatrix();

    GENERATED_BODY();
};