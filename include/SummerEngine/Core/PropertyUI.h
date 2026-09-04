// PropertyDrawer.h
#pragma once
#include <string>
#include <functional>

class EngineObject;

class PropertyUI
{
public:
    virtual ~PropertyUI() = default;

    virtual EngineObject* BuildUI(const std::string& propertyName, const std::string& propertyValue, std::function<void(const std::string&)> setPropertyValue);
    EngineObject* BuildPropertyRow(const std::string& propertyName)
    {
        EngineObject* rowObject = new EngineObject();
        RectTransform* rowRect = rowObject->AddComponent<RectTransform>();
        rowRect->sizeScale = {1, 0};
        rowRect->sizeOffset = {0,25};
        rowRect->pivot = {.5};
        
        UIImage* rowImage = rowObject->AddComponent<UIImage>();
        rowImage->material = EngineUIColors::engineUIMaterials["PanelBackground"];

        EngineObject* nameObject = new EngineObject();
        nameObject->SetParent(rowObject);

        RectTransform* nameTransform = nameObject->AddComponent<RectTransform>();
        nameTransform->sizeScale = {1};
        nameTransform->sizeOffset = {-5,0};
        nameTransform->positionOffset = {5,0};
        nameTransform->pivot = {0};

        UIText* propertyNameText = nameObject->AddComponent<UIText>();
        propertyNameText->material = EngineUIColors::engineUIMaterials["TextBody"];
        propertyNameText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
        propertyNameText->text = propertyName;
        propertyNameText->verticalAlignment = UIAlignmentVertical::Center;
        propertyNameText->horizontalAlignment = UIAlignmentHorizontal::Left;
        propertyNameText->fontSize = 16;

        return rowObject;
    };
};