#include "EngineUI.h"

EngineUI::EngineUI()
{
    EnginePanel* heiarchyPanel = WorkspaceManager::RegisterPanel("Heiarchy", "Editor", Vector2(-1,1));
}