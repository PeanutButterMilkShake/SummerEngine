#pragma once

#include "Component.h"
#include "MathTypes.h"

class RectTransform : public Component
{
public:
    Vector2 position;
    Vector2 size = Vector2(480,100);
    Vector2 pivot = Vector2(0,0);
    Vector2 anchor = Vector2(0,0);

    RectTransform();
};