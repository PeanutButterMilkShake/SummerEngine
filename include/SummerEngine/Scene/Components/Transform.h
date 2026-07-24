
#pragma once 

#include "Component.h"
#include "MathTypes.h"

class Transform : public Component
{
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale = Vector3(1.0f);

    Vector3 forward;
    Vector3 right;
    Vector3 up;

    void Update(float delta) override;
};