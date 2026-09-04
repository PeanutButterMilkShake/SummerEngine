#include "EngineUI.h"
#include "HeirarchyPanel.h"
#include "InspectorPanel.h"
#include "AssetBrowser.h"
#include "WorkspaceManager.h"
#include "EngineObject.h"
#include "Object.h"
#include "PropertyUIRegistry.h"

EngineUI* EngineUI::singleton = nullptr;

EngineUI::EngineUI()
{   
    singleton = this;

    PropertyRegistry::Init();

    EngineUIColors::SetupMaterials();
    WorkspaceManager::SetupEngineUI(); 

    HierarchyPanel* hierarchyPanel = new HierarchyPanel();
    hierarchyPanel->Init();
    
    InspectorPanel* inspectorPanel = new InspectorPanel();
    inspectorPanel->Init();

    AssetBrowser* assetBrowser = new AssetBrowser();
    assetBrowser->Init();

    panels.push_back(assetBrowser);
    panels.push_back(inspectorPanel);
    panels.push_back(hierarchyPanel);
}

void EngineUI::Update()
{
    for(PanelContent* panel : panels)
    {
        panel->Update();
    }
}