#pragma once

#include "PanelContent.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "UIText.h"
#include "EnginePanel.h"

class Object;
class EngineObject;
class UIImage;

class InspectorPanel : public PanelContent
{
public:
    static InspectorPanel* instance;

    InspectorPanel();
    
    void Init() override;
    void Update() override;

private:
    EnginePanel* inspectorPanel;
    std::vector<Object*> lastSelectedObjects;
    UIText* nameText;
};