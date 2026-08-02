#include "RectTransform.h"
#include "Engine.h"

RectTransform::RectTransform()
{
    
}

bool RectTransform::IsMouseHovering()
{
    glm::mat4 inverseMatrix = glm::inverse(GetRectMatrix());
    glm::vec4 localMouse = inverseMatrix * glm::vec4((glm::vec2)Input::GetMousePosition(), 0.0f, 1.0f);

    return (localMouse.x >= -0.5f && localMouse.x <= 0.5f && localMouse.y >= -0.5f && localMouse.y <= 0.5f);
}

glm::mat4 RectTransform::GetRectMatrix()
{
    if(dirtyTransform == this)
        return lastRectMatrix;

    // 1. Centre to window
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(Engine::windowDimensions.x * 0.5f, Engine::windowDimensions.y * 0.5f, 0));

    // 2. Anchor position offset (-1 to 1 range, 0 is center)
    float screenAnchorX = anchor.x * (Engine::windowDimensions.x * 0.5f);
    float screenAnchorY = -anchor.y * (Engine::windowDimensions.y * 0.5f);

    model = glm::translate(model, glm::vec3(screenAnchorX + position.x, screenAnchorY + position.y, 0));

    // 3. Rotation
    model = model * glm::mat4_cast((glm::quat)rotation);

    // 4. Scale 
    Vector2 scaleFactor = (referenceResolution / Engine::windowDimensions);
    Vector2 scaledSize = Vector2(size.x / scaleFactor.y, size.y / scaleFactor.y);
    
    model = glm::scale(model, glm::vec3(scaledSize.x, scaledSize.y, 1.0f));

    float localPivotX = -pivot.x * 0.5f;
    float localPivotY = pivot.y * 0.5f;

    model = glm::translate(model, glm::vec3(localPivotX, localPivotY, 0.0f));

    lastRectMatrix = model;
    return model;
}