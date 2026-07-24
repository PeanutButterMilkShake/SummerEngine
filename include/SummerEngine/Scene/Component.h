#pragma once

class Object;

class Component
{
public:
    Component();
    ~Component();

    Object* object;

    virtual void Start();
    virtual void Update(float delta);
    virtual void SteppedUpdate(float delta);
    virtual void OnDestroy();

    template <typename T>
    T* GetComponent();


    bool hasStarted = false;
};