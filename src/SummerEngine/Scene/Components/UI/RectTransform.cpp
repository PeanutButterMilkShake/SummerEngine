#include "RectTransform.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>

RectTransform::RectTransform() {}

void RectTransform::Update(float delta)
{
    if(IsMouseHovering() && !mouseHovering)
    {
        mouseHovering = true;
        onMouseEnter.Broadcast();
    }
    else if(!IsMouseHovering() && mouseHovering)
    {
        mouseHovering = false;
        onMouseExit.Broadcast();
    }
}

Vector2 RectTransform::GetAbsoluteSize()
{
    RectTransform* parentRect = (object && object->parent) ? object->parent->GetComponent<RectTransform>() : nullptr;
    Vector2 parentSize = parentRect ? parentRect->GetAbsoluteSize() : Engine::windowDimensions;

    // Roblox Size Math: (ParentSize * Scale) + Offset
    return Vector2(
        (parentSize.x * sizeScale.x) + sizeOffset.x,
        (parentSize.y * sizeScale.y) + sizeOffset.y
    );
}

Vector2 RectTransform::GetAbsolutePosition()
{
    RectTransform* parentRect = (object && object->parent) ? object->parent->GetComponent<RectTransform>() : nullptr;
    Vector2 parentSize = parentRect ? parentRect->GetAbsoluteSize() : Engine::windowDimensions;
    Vector2 parentPos = parentRect ? parentRect->GetAbsolutePosition() : Vector2(0.0f, 0.0f);

    // Anchor point: where the pivot sits relative to the parent
    Vector2 anchorPos = parentPos + Vector2(
        (parentSize.x * positionScale.x) + positionOffset.x + positionScrollOffset.x,
        (parentSize.y * positionScale.y) + positionOffset.y + positionScrollOffset.y
    );

    Vector2 mySize = GetAbsoluteSize();

    // Shift back from the anchor by pivot * size, on both axes
    float finalX = anchorPos.x - (mySize.x * pivot.x);
    float finalY = anchorPos.y - (mySize.y * pivot.y);

    return Vector2(finalX, finalY);
}

bool RectTransform::IsMouseHovering()
{
    glm::mat4 inverseMatrix = glm::inverse(GetRectMatrix());
    glm::vec4 localMouse = inverseMatrix * glm::vec4((glm::vec2)Input::GetMousePosition(), 0.0f, 1.0f);
    return (localMouse.x >= -0.5f && localMouse.x <= 0.5f && localMouse.y >= -0.5f && localMouse.y <= 0.5f);
}

glm::mat4 RectTransform::GetRectMatrix()
{
    Vector2 absSize = GetAbsoluteSize();
    Vector2 topLeft = GetAbsolutePosition();
    Vector2 pivotWorldPos = topLeft + Vector2(absSize.x * pivot.x, absSize.y * pivot.y);
    Vector2 centerPos = topLeft + Vector2(absSize.x * 0.5f, absSize.y * 0.5f);
    Vector2 centerRelativeToPivot = centerPos - pivotWorldPos;

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(pivotWorldPos.x, pivotWorldPos.y, 0.0f));
    model = model * glm::mat4_cast((glm::quat)rotation);
    
    model = glm::translate(model, glm::vec3(centerRelativeToPivot.x, centerRelativeToPivot.y, zOffset));
    model = glm::scale(model, glm::vec3(absSize.x, absSize.y, 1.0f));

    lastRectMatrix = model;
    return model;
}