#include "Object.h"
#include "Engine.h"

Object::Object()
{
    id = Engine::objects.size();
    Engine::AddObject(this);
    parent = nullptr;
}

Object::~Object()
{

}

void Object::_SetWorldTransform()
{
    worldPosition = position;
    worldRotation = rotation;
    worldScale = scale;

    if(parent != nullptr)
    {
        parent->_SetWorldTransform();
        worldPosition += parent->worldPosition;
        worldRotation += parent->worldRotation;
        worldScale *= parent->worldScale;
    }
}

void Object::_SceneUpdate()
{
    _SetWorldTransform();

    float pitch = glm::radians(worldRotation.x);
    float yaw = glm::radians(worldRotation.y);
    float roll  = glm::radians(worldRotation.z);

    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);
    forward = glm::normalize(forward);

    glm::vec3 worldUp(0, 1, 0);
    right = glm::normalize(glm::cross(forward, worldUp));

    up = glm::normalize(glm::cross(right, forward));

    Update();
}

void Object::Update() {}