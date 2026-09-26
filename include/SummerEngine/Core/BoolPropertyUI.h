// PropertyDrawer.h
#pragma once
#include "PropertyUI.h"
#include "EngineObject.h"
#include "RectTransform.h"
#include "UIText.h"
#include "UIImage.h"
#include "EngineUI.h"

class BoolPropertyUI : public PropertyUI
{
public:
    EngineObject* BuildUI(const std::string& propertyName, const std::string& propertyValue, std::function<void(const std::string&)> setPropertyValue)
    {
        EngineObject* row = new EngineObject();
        RectTransform* rowRect = row->AddComponent<RectTransform>();
        rowRect->sizeScale = {1, 0};
        rowRect->sizeOffset = {0,25};
        rowRect->pivot = {.5};
        
        UIImage* rowImage = row->AddComponent<UIImage>();
        rowImage->material = Material("UI_PanelBackground");

        UIText* propertyNameText = row->AddComponent<UIText>();
        propertyNameText->material = Material("UI_TextBody");
        propertyNameText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
        propertyNameText->text = propertyName;
        propertyNameText->verticalAlignment = UIAlignmentVertical::Center;
        propertyNameText->horizontalAlignment = UIAlignmentHorizontal::Left;
        propertyNameText->fontSize = 16;

        return row;
    }
};