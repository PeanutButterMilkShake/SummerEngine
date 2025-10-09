#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

class Engine;

class Object
{
public:
    unsigned int id;

    Object *parent;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::vec3 worldPosition = glm::vec3(0.0f);
    glm::vec3 worldRotation = glm::vec3(0.0f);
    glm::vec3 worldScale = glm::vec3(1.0f);

    glm::vec3 forward = glm::vec3(0.0f);
    glm::vec3 right = glm::vec3(0.0f);
    glm::vec3 up = glm::vec3(0.0f);

    Object();
    ~Object();

    void _SetWorldTransform();
    void _SceneUpdate();
    virtual void Update();

private:

};

#endif