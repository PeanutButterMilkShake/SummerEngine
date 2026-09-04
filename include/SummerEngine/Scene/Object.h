#pragma once

#include <glm/glm.hpp>
#include "Component.h"
#include <vector>
#include <format>
#include <algorithm>
#include "Event.h"
#include <typeindex>
#include <unordered_map>

class Engine;
class Component;

class Object
{
public:
    unsigned int id;

    bool isPendingDestroy = false;
    bool enabled = true;

    Event<> onDestroy;

    Object *parent;
    std::vector<Object*> children;

    std::string name;
    std::vector<Component*> components;

    Object();
    Object(bool isEngineObject);
    virtual ~Object();

    void SetParent(Object* newParent);
    Object* GetChildWithName(std::string childName);
    void ClearChildren();
    std::vector<Object*> GetDescendants();

    void Update(float delta);
    void SteppedUpdate(float delta);
    
    // Adding a component to object
    template <typename T>
    T* AddComponent()
    {
        T* newComponent = new T();
        newComponent->object = this;
        components.push_back(newComponent);

        componentData.push_back(std::pair<std::type_index, Component*>(typeid(*newComponent), newComponent));

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

    // Getting an all components of one type
    template <typename T>
    std::vector<T*> GetComponentsOfType()
    {
        std::vector<T*> componentsToReturn;
        for(Component* component : components)
        {
            if(T* casted = dynamic_cast<T*>(component))
            {
                componentsToReturn.push_back(casted);
            }
        }

        return componentsToReturn;
    }

    std::vector<std::pair<std::type_index, std::vector<PropertyInfo>>> GetComponentData();

private:
    std::vector<std::pair<std::type_index, Component*>> componentData;
};

template <typename T>
inline T* Component::GetComponent()
{
    if (!object) return nullptr;
    return object->GetComponent<T>();
}