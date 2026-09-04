#pragma once

#include "PanelContent.h"
#include <string>
#include <unordered_map>

class Object;
class EngineObject;
class UIImage;

class HierarchyPanel : public PanelContent
{
public:
    static HierarchyPanel* instance;

    HierarchyPanel();
    
    void Init() override;
    void Update() override;

    void RefreshHierarchy();
    void UpdateSelectionVisuals();
    
private:
    EngineObject* CreateHeiarchyBranch(Object* object, EngineObject* parent, float indent);
    EngineObject* CreateHeiarchyBranch(std::string name);
    void UpdateObjectHeiarchy(Object* object, EngineObject* parent, float indent);

    EngineObject* sceneRootNode = nullptr;
    std::unordered_map<Object*, UIImage*> hierarchyNodeImages;
};