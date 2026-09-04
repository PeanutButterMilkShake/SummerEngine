#include "EngineObject.h"
#include "Engine.h"

EngineObject::EngineObject() : Object(true)
{
    static unsigned int nextId = 0;
    id = nextId++; 

    Engine::AddEngineObject(this);
    parent = nullptr;
    name = id == 0 ? "Engine Object" : std::format("Engine Object ({})", id);
}

EngineObject::~EngineObject()
{
    onDestroy.Broadcast();

    // Detach from parent
    SetParent(nullptr);

    // Clean up children
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

    // Remove from object vector
    Engine::RemoveEngineObject(this);
}

EngineObject* EngineObject::GetChildWithName(std::string childName)
{
    for(EngineObject* child : children)
    {
        if(child->name == childName)
        {
            return child;
        }
    }

    return nullptr;
}

std::vector<EngineObject*>* GetChildrenRecursive(EngineObject* object, std::vector<EngineObject*>* descendants)
{
    descendants->insert(descendants->end(), object->children.begin(), object->children.end());
    for(EngineObject* child : object->children)
    {
        GetChildrenRecursive(child, descendants);
    }

    return descendants;
}

std::vector<EngineObject*> EngineObject::GetDescendants()
{
    std::vector<EngineObject*> descendants;
    return *GetChildrenRecursive(this, &descendants);
}