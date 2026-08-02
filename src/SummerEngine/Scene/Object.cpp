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
    for (Component* component : components)
    {
        delete component;
    }
    components.clear();
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