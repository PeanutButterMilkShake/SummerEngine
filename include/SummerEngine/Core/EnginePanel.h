#pragma once

#include "MathTypes.h"
#include "UIImage.h"
#include "string.h"
#include "UIText.h"
#include "UIButton.h"
#include "UIListLayout.h"
#include "UIScrollView.h"
#include "EngineObject.h"

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

    EngineObject* tabObject = new EngineObject();
    EngineObject* content = new EngineObject();

    EnginePanel(std::string _title, std::string _category, Vector2 _defaultLocation, std::string iconPath);
};