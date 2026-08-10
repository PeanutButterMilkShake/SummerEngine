#pragma once

#include "EnginePanel.h"
#include "EngineUI.h"
#include "UIListLayout.h"

class WorkspaceManager
{
public:

    inline static std::unordered_map<PanelLocation, Object*> panels = {
        //{PanelLocation::Top, new Object()},
        //{PanelLocation::Bottom, new Object()},
        {PanelLocation::Left, new Object()},
        {PanelLocation::Right, new Object()},
    };

    inline static std::unordered_map<PanelLocation, std::vector<EnginePanel*>> panelsInLocation = {
        {PanelLocation::Top, std::vector<EnginePanel*>()},
        {PanelLocation::Bottom, std::vector<EnginePanel*>()},
        {PanelLocation::Left, std::vector<EnginePanel*>()},
        {PanelLocation::Right, std::vector<EnginePanel*>()},
    };

    static void SetupEngineUI()
    {
        for(std::pair<const PanelLocation, Object*> const& pair : panels)
        {
            PanelLocation location = pair.first;
            Object* panelObject = pair.second;

            // Setup panel positioning
            RectTransform* transform = panelObject->AddComponent<RectTransform>();
            transform->sizeScale = {.2,1};
            if(location == PanelLocation::Left) //TODO add top and bottom
            {
                transform->pivot = {0,0};
                transform->positionScale = {0,0};
            }
            else if(location == PanelLocation::Right)
            {
                transform->pivot = {1,0};
                transform->positionScale = {1,0};
            }
            
            transform->sizeOffset = {0,0};
            
            // Add panel image
            panelObject->AddComponent<UIImage>()->material = EngineUIColors::engineUIMaterials["PanelBackground"];
            
            // Create tab ribbon
            Object* tabRibbon = new Object();
            tabRibbon->name = "Ribbon";
            tabRibbon->SetParent(panelObject);

            RectTransform* ribbonTransform = tabRibbon->AddComponent<RectTransform>();
            ribbonTransform->sizeScale = {1,0};
            ribbonTransform->sizeOffset = {0,25};
            ribbonTransform->pivot = {0,0};

            UIImage* ribbon = tabRibbon->AddComponent<UIImage>();
            ribbon->material = EngineUIColors::engineUIMaterials["PanelRibbon"];

            tabRibbon->AddComponent<UIListLayout>()->paddingOffset.x = 2;

            panelObject->enabled = false;
        }
    }

    static EnginePanel* RegisterPanel(std::string title, std::string category, PanelLocation defaultLocation, std::string iconPath)
    {
        return RegisterPanel(title, category, defaultLocation, iconPath, false);
    }

    static EnginePanel* RegisterPanel(std::string title, std::string category, PanelLocation defaultLocation, std::string iconPath, bool enabled)
    {
        EnginePanel* newPanel = new EnginePanel(title, category, defaultLocation, iconPath);

        // Add panel to current panels in dock location
        panelsInLocation[defaultLocation].push_back(newPanel);
        panels[defaultLocation]->enabled = true;
    
        newPanel->tabObject->SetParent(panels[defaultLocation]->GetChildWithName("Ribbon"));
        newPanel->tabObject->enabled = true;

        return newPanel;
    }
};


// Helper macros
/*#define REGISTER_ENGINE_COMMAND(name, menu, func) \ 
    static struct UniqueEngineFunc_##__LINE__ { \
        UniqueReg_##__LINE__() { \
            RegisterCommand(name, menu, func); \
        } \
    } unique_reg_instance_##__LINE__;

#define REGISTER_ENGINE_MENU(name, keybind, editorPanel) \ 
    static struct UniqueEngineMenu_##__LINE__ { \
        UniqueReg_##__LINE__() { \
            RegisterMenu(name, keybind, editorPanel); \
        } \
    } unique_reg_instance_##__LINE__;
*/