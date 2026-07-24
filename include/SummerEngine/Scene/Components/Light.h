#pragma once

#include "MathTypes.h"
#include "Component.h"
#include "Engine.h"

enum LightType
{
    Directional, // 0
    Spot,        // 1
    Point,       // 2
    Area         // 3
};

class Light : public Component
{
public:
    Light();
    ~Light();

    Color3 color = Color3(1,1,1);
    float strength = 1;
    LightType type = LightType::Directional;
};