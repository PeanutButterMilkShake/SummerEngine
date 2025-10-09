#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"

class Engine;

class Camera : public Object
{
public:
    float fov = 40;
    float width = 800;
    float height = 600;
    float nearPlane = 0.1;
    float farPlane = 100;

    Camera();
    ~Camera();

    glm::mat4 ViewMatrix()
    {
        glm::vec3 target = worldPosition + glm::normalize(forward);
        return glm::lookAt(worldPosition, target, glm::vec3(0, 1, 0));
    }

    glm::mat4 PerspectiveMatrix()
    {
        float aspect = width / height;
        return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
    }

private:

};

#endif