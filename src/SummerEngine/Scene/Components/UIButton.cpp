#include "UIButton.h"

void UIButton::Start()
{
    transform = GetComponent<RectTransform>();
}

void UIButton::Update(float delta)
{
    //Set button states
    if(Input::GetMouse0Down() && transform->IsMouseHovering())
    {
        if(leftMouseState == UIButtonState::Press || leftMouseState == UIButtonState::Down)
            leftMouseState = UIButtonState::Down;
        else
            leftMouseState = UIButtonState::Press;
    }
    else if(leftMouseState == UIButtonState::Press || leftMouseState == UIButtonState::Down)
    {
        leftMouseState = UIButtonState::Up;
    }

    if(Input::GetMouse1Down() && transform->IsMouseHovering())
    {
        if(rightMouseState == UIButtonState::Press || leftMouseState == UIButtonState::Down)
            rightMouseState = UIButtonState::Down;
        else
            rightMouseState = UIButtonState::Press;
    }
    else if(rightMouseState == UIButtonState::Press || rightMouseState == UIButtonState::Down)
    {
        rightMouseState = UIButtonState::Up;
    }

    // Event broadcasting
    if(leftMouseState == UIButtonState::Press)
    {
        OnMouse0PressEvent.Broadcast();
        OnMouse0DownEvent.Broadcast();
    }
    else if(leftMouseState == UIButtonState::Down)
    {
        OnMouse0DownEvent.Broadcast();
    }
    else if(leftMouseState == UIButtonState::Up)
    {
        leftMouseState = UIButtonState::None;
        OnMouse0ReleasedEvent.Broadcast();
    }

    if(rightMouseState == UIButtonState::Press)
    {
        OnMouse1PressEvent.Broadcast();
        OnMouse1DownEvent.Broadcast();
    }
    else if(rightMouseState == UIButtonState::Down)
    {
        OnMouse1DownEvent.Broadcast();
    }
    else if(rightMouseState == UIButtonState::Up)
    {
        rightMouseState = UIButtonState::None;
        OnMouse1ReleasedEvent.Broadcast();
    }
}