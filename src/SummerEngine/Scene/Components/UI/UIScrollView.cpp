#include "UIScrollView.h"
#include <algorithm>

void UIScrollView::Start()
{
    transform = GetComponent<RectTransform>();
}

void UIScrollView::Update(float delta)
{
    if(!transform->IsMouseHovering())
        return;

    Vector2 wheelDelta = Input::GetScrollDirection();

    if (scrollDirection == UIAxis::Horizontal)
    {
        // Use vertical wheel (wheelDelta.y) for horizontal movement
        currentScrollOffset.x += wheelDelta.y * scrollSpeed;
        currentScrollOffset.y = 0.0f;
    }
    else if (scrollDirection == UIAxis::Vertical)
    {
        currentScrollOffset.y += wheelDelta.y * scrollSpeed;
        currentScrollOffset.x = 0.0f;
    }
    else if (scrollDirection == UIAxis::Both)
    {
        currentScrollOffset.x += wheelDelta.x * scrollSpeed;
        currentScrollOffset.y += wheelDelta.y * scrollSpeed;
    }

    Vector2 viewportSize = transform->GetAbsoluteSize();
    float sign = invertedScrollView ? -1.0f : 1.0f;
    Vector2 bounds = viewportSize * scrollViewSize * sign;

    Vector2 minBounds = Vector2(std::min(0.0f, bounds.x), std::min(0.0f, bounds.y));
    Vector2 maxBounds = Vector2(std::max(0.0f, bounds.x), std::max(0.0f, bounds.y));

    currentScrollOffset = Clamp(currentScrollOffset, minBounds, maxBounds);

    for (Object* child : object->children)
    {
        if (RectTransform* childTransform = child->GetComponent<RectTransform>())
        {
            childTransform->positionScrollOffset = currentScrollOffset;
        }
    }
}