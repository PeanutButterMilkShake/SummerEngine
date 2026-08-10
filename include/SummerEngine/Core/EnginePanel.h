#pragma once

#include "MathTypes.h"
#include "Object.h"
#include "UIImage.h"
#include "string.h"
#include "UIText.h"
#include "UIButton.h"

enum PanelLocation
{
    Top,
    Bottom,
    Left,
    Right
};

class EnginePanel
{
public:
    std::string title;
    std::string category;
    Vector2 defaultLocation;

    Object* tabObject = new Object();
    Object* content = new Object();

    EnginePanel(std::string _title, std::string _category, Vector2 _defaultLocation, std::string iconPath);
};