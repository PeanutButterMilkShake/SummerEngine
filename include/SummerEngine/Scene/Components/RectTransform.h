#pragma once

#include "Component.h"
#include "MathTypes.h"
#include <glm.hpp>

class RectTransform : public Component
{
public:
    Vector2 position;
    Vector2 size = Vector2(100,100);
    Vector2 pivot = Vector2(0);
    Vector2 anchor = Vector2(0,0);
    Quaternion rotation;

    RectTransform();

    bool IsMouseHovering();
    glm::mat4 GetRectMatrix();

    bool operator==(const RectTransform& other) const
    {
        return other.pivot == pivot && other.anchor == anchor && other.position == position && other.size == size;
    }

private:
    glm::mat4 lastRectMatrix;
    RectTransform* dirtyTransform;
    Vector2 referenceResolution = Vector2(960, 600); // remove later for UI container
};