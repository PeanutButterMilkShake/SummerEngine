// PropertyDrawer.h
#pragma once
#include "PropertyUI.h"
#include "EngineObject.h"
#include "RectTransform.h"
#include "UIText.h"
#include "UIImage.h"
#include "EngineUI.h"
#include "UITextField.h"

class FloatPropertyUI : public PropertyUI
{
public:
    EngineObject* BuildUI(const std::string& propertyName, const std::string& propertyValue, std::function<void(const std::string&)> setPropertyValue)
    {
        EngineObject* rowObject = BuildPropertyRow(propertyName);

        EngineObject* propertyObject = new EngineObject();
        propertyObject->SetParent(rowObject);

        RectTransform* propertyTransform = propertyObject->AddComponent<RectTransform>();
        propertyTransform->sizeScale = {.5, 1};
        propertyTransform->sizeOffset = {-5,0};
        propertyTransform->positionOffset = {5,0};
        propertyTransform->positionScale = {.4,0};
        propertyTransform->pivot = {0};

        UIImage* propertyBackground = propertyObject->AddComponent<UIImage>();
        propertyBackground->material = EngineUIColors::engineUIMaterials["PanelRibbon"];

        UIButton* propertyButton = propertyObject->AddComponent<UIButton>();    
        UITextField* propertyTextField = propertyObject->AddComponent<UITextField>();
        propertyTextField->fieldType = FieldType::Numbers;

        EngineObject* propertyTextObject = new EngineObject();
        propertyTextObject->SetParent(propertyObject);

        RectTransform* propertyTextTransform = propertyTextObject->AddComponent<RectTransform>();
        propertyTextTransform->sizeScale = {1};
        propertyTextTransform->sizeOffset = {0};
        propertyTextTransform->pivot = {0};
        
        UIText* propertyText = propertyTextObject->AddComponent<UIText>();
        propertyText->material = EngineUIColors::engineUIMaterials["TextBody"];
        propertyText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
        propertyText->verticalAlignment = UIAlignmentVertical::Center;
        propertyText->horizontalAlignment = UIAlignmentHorizontal::Left;
        propertyText->fontSize = 16;

        propertyTextField->textObject = propertyText;

        float value = std::stof(propertyValue);
        std::string newString;
        if (value != (int)value)
        {
            int lastNonZero = 0;
            for(int i = 0; i < propertyValue.length(); i++)
            {
                if(propertyValue.at(i) != '0')
                {
                    lastNonZero = i;
                }
            }

            newString = std::to_string(value).substr(0, lastNonZero + 1);
        }
        else
        {
            newString = std::to_string((int)value);
        }

        propertyTextField->text = newString;

        propertyTextField->onSubmit.AddListener([propertyTextField, setPropertyValue](){
            setPropertyValue(propertyTextField->text);
        });

        return rowObject;
    }
};