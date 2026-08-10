#include "Object.h"
#include "Engine.h"
#include "Component.h"

Object::Object()
{
    id = Engine::objects.size();
    Engine::AddObject(this);
    parent = nullptr;
    name = id == 0 ? "New object" : std::format("New object ({})", id);
}

Object::~Object()
{
    // Detach from parent first
    SetParent(nullptr);

    // Clean up all children recursively
    for (Object* child : children)
    {
        delete child;
    }
    children.clear();

    // Delete components
    for (Component* component : components)
    {
        delete component;
    }
    components.clear();
}

void Object::SetParent(Object* newParent)
{
    if (parent == newParent) return;

    // Remove this object from its current parent's children list
    if (parent != nullptr)
    {
        auto& siblings = parent->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    }

    parent = newParent;

    // Add this object to the new parent's children list
    if (parent != nullptr)
    {
        parent->children.push_back(this);
    }
}

Object* Object::GetChildWithName(std::string childName)
{
    for(Object* child : children)
    {
        if(child->name == childName)
        {
            return child;
        }
    }

    return nullptr;
}

void Object::Update(float delta)
{
    for(Component* component : components)
    {
        if(!component->hasStarted)
        {
            component->Start();
            component->hasStarted = true;
        }
            
        component->Update(delta);
    }
}

void Object::SteppedUpdate(float delta)
{
    for(Component* component : components)
    {
        component->SteppedUpdate(delta);
    }
}