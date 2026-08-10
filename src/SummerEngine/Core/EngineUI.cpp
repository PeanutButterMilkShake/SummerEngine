#include "EngineUI.h"
#include "WorkspaceManager.h"

EngineUI::EngineUI()
{
    EngineUIColors::SetupMaterials();
    WorkspaceManager::SetupEngineUI();

    EnginePanel* heiarchyPanel = WorkspaceManager::RegisterPanel("Heiarchy", "Editor", PanelLocation::Left, "Assets/Textures/Heiarchy.png", true);
    EnginePanel* propertiesPanel = WorkspaceManager::RegisterPanel("Inspector", "Editor", PanelLocation::Left, "Assets/Textures/Inspector.png", true);
}   