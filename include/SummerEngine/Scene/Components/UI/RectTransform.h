#pragma once
#include "Component.h"
#include "MathTypes.h"
#include "Object.h"
#include <glm.hpp>
#include "Event.h"
#include "RectTransform.h.generated.h"
class RectTransform : public Component
{
public:
    Event<> onMouseEnter;
    Event<> onMouseExit;

    SPROPERTY();
    Vector2 sizeScale = Vector2(0.0f, 0.0f);
    
    SPROPERTY();
    Vector2 sizeOffset = Vector2(100.0f, 100.0f);

    SPROPERTY();
    Vector2 positionScale = Vector2(0.0f, 0.0f);

    SPROPERTY();
    Vector2 positionOffset = Vector2(0.0f, 0.0f);

    SPROPERTY();
    Vector2 pivot = Vector2(.5,.5);

    SPROPERTY();
    float zOffset = 0.0f;
    
    Quaternion rotation;

    Vector2 positionScrollOffset = Vector2(0.0f, 0.0f); // Offset for UIScrollView

    // fragment shader clipping settings
    bool clipChildren = false;

    RectTransform();

    void Update(float delta) override;

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
    bool mouseHovering = false;

    GENERATED_BODY();
};