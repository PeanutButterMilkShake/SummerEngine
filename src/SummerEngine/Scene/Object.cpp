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

Object::Object(bool isEditorObject)
{
    parent = nullptr;
}

Object::~Object()
{
    onDestroy.Broadcast();

    // 1. Detach from parent safely
    if (parent != nullptr)
    {
        auto& siblings = parent->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        parent = nullptr;
    }

    // 2. Copy children and clear the vector first to prevent iterator invalidation
    std::vector<Object*> tempChildren = children;
    children.clear();
    
    for (Object* child : tempChildren)
    {
        child->parent = nullptr; // Prevent child from trying to erase itself from our cleared vector
        delete child;
    }

    // 3. Delete components
    for (Component* component : components)
    {
        delete component;
    }
    components.clear();

    Engine::RemoveObject(this);
}

void Object::SetParent(Object* newParent)
{
    if (parent == newParent) return;

    if (parent != nullptr)
    {
        auto& siblings = parent->children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    }

    parent = newParent;

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

std::vector<Object*>* GetChildrenRecursive(Object* object, std::vector<Object*>* descendants)
{
    descendants->insert(descendants->end(), object->children.begin(), object->children.end());
    for(Object* child : object->children)
    {
        GetChildrenRecursive(child, descendants);
    }

    return descendants;
}

std::vector<Object*> Object::GetDescendants()
{
    std::vector<Object*> descendants;
    return *GetChildrenRecursive(this, &descendants);
}

void Object::ClearChildren()
{
    std::vector<Object*> tempChildren = children;
    children.clear();
    
    for(Object* child : tempChildren)
    {
        child->parent = nullptr;
        delete child;
    }
}


void Object::Update(float delta)
{
    std::vector<Component*> activeComponents = components;

    for (Component* component : activeComponents)
    {
        if (!component->hasStarted)
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

std::vector<std::pair<std::type_index, std::vector<PropertyInfo>>> Object::GetComponentData()
{
    std::vector<std::pair<std::type_index, std::vector<PropertyInfo>>> componentDataToSend;

    for(std::pair<std::type_index, Component*> component: componentData)
    {   
        componentDataToSend.push_back(std::pair<std::type_index, std::vector<PropertyInfo>>(component.first, component.second->GetProperties()));
    }

    return componentDataToSend;
}