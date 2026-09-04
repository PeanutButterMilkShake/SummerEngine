#pragma once 

#include "Component.h"
#include "MathTypes.h"
#include "Transform.h.generated.h"
class Transform : public Component
{
public:
    SPROPERTY();
    Vector3 position;
    
    Quaternion rotation;
    
    SPROPERTY();
    Vector3 scale = Vector3(1.0f);

    Vector3 forward;
    Vector3 right;
    Vector3 up;

    void Update(float delta) override;

    GENERATED_BODY();
};