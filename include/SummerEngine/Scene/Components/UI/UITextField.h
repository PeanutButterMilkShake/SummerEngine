#pragma once

#include "Component.h"
#include "RectTransform.h"
#include "UIText.h"
#include "UIButton.h"
#include "Object.h"

enum FieldType
{
    Any,
    Text,
    Numbers,
};

class UITextField : public Component
{
public:
    Event<> onSubmit;

    FieldType fieldType;
    std::string text = "";
    UIText* textObject;

    UITextField() {};

    void Start() override;
    void Update(float delta) override;
    void OnFieldClicked();

private:
    RectTransform* transform;
    UIButton* button;
    bool isTyping = false;
    float typeCooldown = .2;
    float cooldown = 0;
    KeyCode lastKeyPressed = KeyCode::NONE;
};