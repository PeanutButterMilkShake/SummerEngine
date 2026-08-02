#pragma once

#include <glm/glm.hpp>
#include "Component.h"
#include <vector>
#include <format>

class Engine;
class Component;

class Object
{
public:
    unsigned int id;

    Object *parent;

    std::string name;
    std::vector<Component*> components;

    Object();
    ~Object();

    void Update(float delta);
    void SteppedUpdate(float delta);
    
    // Adding a component to object
    template <typename T>
    T* AddComponent()
    {
        T* newComponent = new T();
        newComponent->object = this;
        components.push_back(newComponent);
        return newComponent;
    }
    
    // Getting an objects component
    template <typename T>
    T* GetComponent()
    {
        for(Component* component : components)
        {
            if(T* casted = dynamic_cast<T*>(component))
            {
                return casted;
            }
        }

        return nullptr;
    }
};

template <typename T>
inline T* Component::GetComponent()
{
    if (!object) return nullptr;
    return object->GetComponent<T>();
}