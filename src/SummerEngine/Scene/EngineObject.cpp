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
    for(Object* childObj : children)
    {
        if (EngineObject* child = dynamic_cast<EngineObject*>(childObj))
        {
            if(child->name == childName)
            {
                return child;
            }
        }
    }

    return nullptr;
}

std::vector<EngineObject*>* GetChildrenRecursive(Object* object, std::vector<EngineObject*>* descendants)
{
    for(Object* childObj : object->children)
    {
        if (EngineObject* child = dynamic_cast<EngineObject*>(childObj))
        {
            descendants->push_back(child);
            GetChildrenRecursive(child, descendants);
        }
    }

    return descendants;
}

std::vector<EngineObject*> EngineObject::GetDescendants()
{
    std::vector<EngineObject*> descendants;
    return *GetChildrenRecursive(this, &descendants);
}