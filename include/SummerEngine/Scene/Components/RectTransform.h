#pragma once

#include "Component.h"
#include "MathTypes.h"
#include "Object.h"
#include <glm.hpp>

class RectTransform : public Component
{
public:
    Vector2 sizeScale = Vector2(0.0f, 0.0f);
    Vector2 sizeOffset = Vector2(100.0f, 100.0f);

    Vector2 positionScale = Vector2(0.0f, 0.0f);
    Vector2 positionOffset = Vector2(0.0f, 0.0f);

    Vector2 pivot = Vector2(.5,.5); 

    Quaternion rotation;

    float zOffset = 0.0f;

    RectTransform();

    bool IsMouseHovering();
    glm::mat4 GetRectMatrix();

    Vector2 GetAbsoluteSize();
    Vector2 GetAbsolutePosition();

    bool operator==(const RectTransform& other) const
    {
        return other.pivot == pivot && 
               other.sizeScale == sizeScale && other.sizeOffset == sizeOffset && 
               other.positionScale == positionScale && other.positionOffset == positionOffset;
    }

private:
    glm::mat4 lastRectMatrix;
    RectTransform* dirtyTransform;
};