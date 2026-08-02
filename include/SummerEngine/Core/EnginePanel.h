#pragma once

#include "MathTypes.h"
#include "Object.h"
#include "UIImage.h"
#include "string.h"

class EnginePanel
{
public:
    std::string title;
    std::string category;
    Vector2 defaultLocation;

    Object panelObject;
    UIImage* panelBackground;

    EnginePanel(std::string _title, std::string _category, Vector2 _defaultLocation);
};