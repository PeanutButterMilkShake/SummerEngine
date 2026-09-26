#pragma once

#include "EngineObject.h"
#include "Event.h"
#include "RectTransform.h"
#include "UIButton.h"

class UIDraggable : public Component
{
public:
    Event<> OnDragStart;
    Event<Object> OnDragEnd;

    RectTransform* transform;
    UIButton* button;

    Object draggedUI;
    bool isBeingDragged = false;

    UIDraggable();

    void Update(float delta) override;
    void Start() override;

private:
    bool isMouseDown = false;
};