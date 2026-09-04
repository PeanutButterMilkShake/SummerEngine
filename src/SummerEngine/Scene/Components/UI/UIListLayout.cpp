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

    Vector2 parentSize = transform->GetAbsoluteSize(); 
    Vector2 lastPosition = {0, 0};
    switch (horizontalAlignment)
    {
        case UIAlignmentHorizontal::Left:
            lastPosition.x = 0.0f;
            break;
        case UIAlignmentHorizontal::Center:
            lastPosition.x = parentSize.x * 0.5f;
            break;
        case UIAlignmentHorizontal::Right:
            lastPosition.x = parentSize.x;
            break;
    }

    switch (verticalAlignment)
    {
        case UIAlignmentVertical::Top:
            lastPosition.y = 0.0f;
            break;
        case UIAlignmentVertical::Center:
            lastPosition.y = parentSize.y * 0.5f;
            break;
        case UIAlignmentVertical::Bottom:
            lastPosition.y = parentSize.y;
            break;
    }

    for(Object* child : lastChildren)
    {
        RectTransform* childTransform = child->GetComponent<RectTransform>();
        Vector2 absoluteSize = childTransform->GetAbsoluteSize();

        childTransform->positionOffset = lastPosition;
        childTransform->positionScale = {0}; // Change to work around pivot and position scale

        if (layoutAxis == UIAxis::Horizontal)
        {
            lastPosition.x += absoluteSize.x + paddingOffset.x + (parentSize.x * paddingScale.x);
        }
        else if (layoutAxis == UIAxis::Vertical)
        {
            lastPosition.y += absoluteSize.y + paddingOffset.y + (parentSize.y * paddingScale.y);
        }
    }
}