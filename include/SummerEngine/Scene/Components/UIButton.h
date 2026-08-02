#pragma once

#include "RectTransform.h"
#include "Component.h"
#include "Object.h"
#include "Engine.h"
#include "Event.h"

enum UIButtonState 
{
    None,
    Up,
    Press,
    Down,
};

class UIButton : public Component
{
public:
    RectTransform* transform;
    UIButtonState leftMouseState;
    UIButtonState rightMouseState;

    Event<> OnMouse0PressEvent;
    Event<> OnMouse0ReleasedEvent;
    Event<> OnMouse0DownEvent;
    Event<> OnMouse1PressEvent;
    Event<> OnMouse1ReleasedEvent;
    Event<> OnMouse1DownEvent;

    void Start() override;
    void Update(float delta) override;
};