// PropertyDrawer.h
#pragma once
#include "PropertyUI.h"
#include "EngineObject.h"
#include "RectTransform.h"
#include "UIText.h"
#include "UIImage.h"
#include "EngineUI.h"

class Vector2PropertyUI : public PropertyUI
{
public:
    EngineObject* BuildUI(const std::string& propertyName, const std::string& propertyValue, std::function<void(const std::string&)> setPropertyValue)
    {
        EngineObject* rowObject = BuildPropertyRow(propertyName);
        
        float x = 0.0f, y = 0.0f;
        sscanf(propertyValue.c_str(), "%f,%f", &x, &y);
        
        UITextField* textFields[2] = { nullptr, nullptr };

        for(int i=0; i < 2; i++)
        {
            float currentElement = x;
            if(i==1)
            {
                currentElement = y;
            }

            EngineObject* propertyObject = new EngineObject();
            propertyObject->SetParent(rowObject);

            RectTransform* propertyTransform = propertyObject->AddComponent<RectTransform>();
            propertyTransform->sizeScale = {.25, 1};
            propertyTransform->sizeOffset = {-5,0};
            propertyTransform->positionOffset = {5,0};
            propertyTransform->positionScale = {.4f + (.25f * i), 0};
            propertyTransform->pivot = {0};

            UIImage* propertyBackground = propertyObject->AddComponent<UIImage>();
            propertyBackground->material = Material("UI_PanelRibbon");

            UIButton* propertyButton = propertyObject->AddComponent<UIButton>();    
            UITextField* propertyTextField = propertyObject->AddComponent<UITextField>();
            propertyTextField->fieldType = FieldType::Numbers;
            textFields[i] = propertyTextField;

            EngineObject* propertyTextObject = new EngineObject();
            propertyTextObject->SetParent(propertyObject);

            RectTransform* propertyTextTransform = propertyTextObject->AddComponent<RectTransform>();
            propertyTextTransform->sizeScale = {1};
            propertyTextTransform->sizeOffset = {0};
            propertyTextTransform->positionOffset = {5,0};
            propertyTextTransform->pivot = {0};
            
            UIText* propertyText = propertyTextObject->AddComponent<UIText>();
            propertyText->material = Material("UI_TextBody");
            propertyText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
            propertyText->verticalAlignment = UIAlignmentVertical::Center;
            propertyText->horizontalAlignment = UIAlignmentHorizontal::Left;
            propertyText->fontSize = 16;
            propertyTextField->textObject = propertyText;

            std::string stringValue = std::to_string(currentElement);
            std::string newString;
            if (currentElement != (int)currentElement)
            {
                int lastNonZero = 0;
                for(int j = 0; j < stringValue.length(); j++)
                {
                    if(stringValue.at(j) != '0')
                    {
                        lastNonZero = j;
                    }
                }

                newString = std::to_string(currentElement).substr(0, lastNonZero + 1);
            }
            else
            {
                newString = std::to_string((int)currentElement);
            }

            propertyTextField->text = newString;
        }

        UITextField* fieldX = textFields[0];
        UITextField* fieldY = textFields[1];

        auto submitVector = [fieldX, fieldY, setPropertyValue]() 
        {
            if (fieldX && fieldY)
            {
                std::string combined = fieldX->text + "," + fieldY->text;
                setPropertyValue(combined);
            }
        };

        if (fieldX) fieldX->onSubmit.AddListener(submitVector);
        if (fieldY) fieldY->onSubmit.AddListener(submitVector);

        return rowObject;
    }
};