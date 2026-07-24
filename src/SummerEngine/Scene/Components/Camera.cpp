#include "Camera.h"
#include "Engine.h"

void Camera::Start()
{
    if(Engine::mainCamera == nullptr)
        Engine::mainCamera = this;
    transform = GetComponent<Transform>();
}

glm::mat4 Camera::ViewMatrix()
{
    glm::vec3 target = transform->position + glm::normalize((glm::vec3)transform->forward);
    return glm::lookAt((glm::vec3)transform->position, target, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::PerspectiveMatrix()
{
    float aspect = Engine::windowDimensions.x / Engine::windowDimensions.y;
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}