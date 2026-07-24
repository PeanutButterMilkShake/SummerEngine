#include "Object.h"
#include "Engine.h"
#include "Component.h"

Object::Object()
{
    id = Engine::objects.size();
    Engine::AddObject(this);
    parent = nullptr;
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
        component->SteppedUpdate(delta); //Change to component->SteppedUpdate
    }
}