#include "UIListLayout.h"

void UIListLayout::Start()
{
    transform = GetComponent<RectTransform>();
    lastChildren = object->children;
}

void UIListLayout::Update(float delta)
{
    if(lastChildren != object->children)
    {
        lastChildren = object->children;
    } 

    CalculatePositions();
}

void UIListLayout::CalculatePositions()
{
    Vector2 lastPosition = {0,0};
    for(Object* child : lastChildren)
    {
        RectTransform* childTransform = child->GetComponent<RectTransform>();
        Vector2 absoluteSize = childTransform->GetAbsoluteSize();

        childTransform->positionOffset = lastPosition;
        lastPosition.x += absoluteSize.x + paddingOffset.x + (transform->GetAbsoluteSize().x * paddingScale.x);
    }
}