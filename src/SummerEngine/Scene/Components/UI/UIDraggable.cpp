#include "UIDraggable.h"

void UIDraggable::Start()
{
    transform = GetComponent<RectTransform>();
    button = GetComponent<UIButton>();
}

void UIDraggable::Update(float delta)
{
    button->OnMouse0DownEvent.AddListener([this]()
    {
        isMouseDown = true;
    });

    button->OnMouse0ReleasedEvent.AddListener([this]()
    {
        isMouseDown = false;
    });

    if(!transform->IsMouseHovering() and isMouseDown)
    {
        printf("drag\n");
    }
}