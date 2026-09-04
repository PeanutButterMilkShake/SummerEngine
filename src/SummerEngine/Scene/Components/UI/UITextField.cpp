#include "UITextField.h"

void UITextField::Start()
{
    transform = GetComponent<RectTransform>();
    button = GetComponent<UIButton>();
    button->OnMouse0ReleasedEvent.AddListener(this, &UITextField::OnFieldClicked);
}

void UITextField::Update(float delta)
{
    textObject->text = text;
    cooldown -= delta;

    if(Input::GetMouse0Down() && !transform->IsMouseHovering())
    {
        isTyping = false;
    }

    if(!isTyping)
        return;

    if (cooldown > 0.0)
    {
        cooldown -= delta;
    }
    
    KeyCode key = Input::GetCurrentKey();
    if(key != KeyCode::NONE && cooldown <= 0.0)
    {
        if(key == KeyCode::Enter)
        {
            isTyping = false;
            onSubmit.Broadcast();
        }
        if(key == KeyCode::BackSpace && text != "")
        {
            text = text.erase(text.length() - 1);
            cooldown = typeCooldown;
        }
        else
        {
            if(key != KeyCode::Period && (fieldType == FieldType::Numbers && !std::isdigit(static_cast<unsigned char>(Input::GetKeyName(key)[0]))))
            {
                return;
            }

            text += Input::GetCurrentPrintableKey();
        }
    }
}


void UITextField::OnFieldClicked()
{
    if(isTyping == true)
        return;

    isTyping = true;
}