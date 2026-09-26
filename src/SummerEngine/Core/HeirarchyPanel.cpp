#include "HeirarchyPanel.h"
#include "EngineUI.h"
#include "WorkspaceManager.h"
#include "EngineObject.h"
#include "Object.h"
#include <algorithm>

using namespace std;

HierarchyPanel* HierarchyPanel::instance = nullptr;

HierarchyPanel::HierarchyPanel()
{
    instance = this;
}

void HierarchyPanel::Init()
{
    objectIcon = Texture("Assets/Textures/Object.png");

    EnginePanel* heirarchyPanel = WorkspaceManager::RegisterPanel("Hierarchy", "Editor", PanelLocation::Left, "Assets/Textures/Hierarchy.png", true);
    heirarchyPanel->content->SetParent(WorkspaceManager::panels[PanelLocation::Left]);
    heirarchyPanel->content->enabled = true;

    sceneRootNode = CreateHeiarchyBranch("Scene");
    sceneRootNode->SetParent(heirarchyPanel->content);
}

void HierarchyPanel::Update()
{
    if(Engine::heirarchyDirty)
    {
        RefreshHierarchy();
        Engine::heirarchyDirty = false;
    }
}

EngineObject* HierarchyPanel::CreateHeiarchyBranch(Object* object, EngineObject* parent, float indent)
{
    EngineObject* branch = new EngineObject();
    branch->SetParent(parent);

    RectTransform* branchTransform = branch->AddComponent<RectTransform>();
    branchTransform->sizeScale = {.97, 0};
    branchTransform->sizeOffset = {0, 20};
    branchTransform->positionScale = {0, 0};
    branchTransform->positionOffset = {0, 0};
    branchTransform->clipChildren = false;
    branchTransform->pivot = {0.5, 0};
    
    UIImage* branchImage = branch->AddComponent<UIImage>();
    
    bool isSelected = std::find(Engine::selectedObjects.begin(), Engine::selectedObjects.end(), object) != Engine::selectedObjects.end();
    branchImage->material = isSelected ? Material("UI_ButtonPress") : Material("UI_PanelBackground");

    instance->hierarchyNodeImages[object] = branchImage;

    branchTransform->onMouseEnter.AddListener([branchImage, object]()
    {
        bool selected = std::find(Engine::selectedObjects.begin(), Engine::selectedObjects.end(), object) != Engine::selectedObjects.end();
        if(selected) return;
        branchImage->material = Material("UI_ButtonHighlight");
    });

    branchTransform->onMouseExit.AddListener([branchImage, object]()
    {
        bool selected = std::find(Engine::selectedObjects.begin(), Engine::selectedObjects.end(), object) != Engine::selectedObjects.end();
        if(selected) return;
        branchImage->material = Material("UI_PanelBackground");
    });

    branch->AddComponent<UIButton>()->OnMouse0ReleasedEvent.AddListener([object]()
    {
        Engine::selectedObjects.clear();
        Engine::selectedObjects.push_back(object);

        instance->UpdateSelectionVisuals();
    });

    EngineObject* textObject = new EngineObject();
    textObject->SetParent(branch);

    RectTransform* textTransform = textObject->AddComponent<RectTransform>();
    textTransform->sizeScale = {1, 1};
    textTransform->pivot = {0};
    textTransform->sizeOffset = {-indent + 6, 0};
    textTransform->positionOffset = {indent + 6, 0};
    textTransform->positionScale = {0};

    UIText* tabText = textObject->AddComponent<UIText>();
    tabText->material = Material("UI_TextBody");
    tabText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    tabText->text = object->name;
    tabText->verticalAlignment = UIAlignmentVertical::Center;
    tabText->horizontalAlignment = UIAlignmentHorizontal::Left;
    tabText->fontSize = 16;

    EngineObject* iconObject = new EngineObject();
    iconObject->SetParent(branch);

    RectTransform* iconTransform = iconObject->AddComponent<RectTransform>();
    iconTransform->pivot = {0};
    iconTransform->sizeOffset = {24, 24};
    iconTransform->sizeScale = {0, 0};
    iconTransform->positionOffset = {indent - 20, 0};
    iconTransform->positionScale = {0};
    
    UIImage* icon = iconObject->AddComponent<UIImage>();
    icon->material = Material("UI_Image");
    icon->texture = objectIcon;

    return branch;
}

EngineObject* HierarchyPanel::CreateHeiarchyBranch(string name)
{
    EngineObject* branch = new EngineObject();
    RectTransform* branchTransform = branch->AddComponent<RectTransform>();
    branchTransform->sizeScale = {.97, 0};
    branchTransform->sizeOffset = {0, 24};
    branchTransform->positionScale = {0, 0};
    branchTransform->positionOffset = {0, 0};
    branchTransform->clipChildren = false;
    branchTransform->pivot = {0.5, 0};
    
    UIImage* branchImage = branch->AddComponent<UIImage>();
    branchImage->material = Material("UI_PanelBackground");

    branch->AddComponent<UIButton>();

    branch->SetParent(WorkspaceManager::GetPanel("Hierarchy")->content);

    EngineObject* textObject = new EngineObject();
    textObject->SetParent(branch);

    RectTransform* textTransform = textObject->AddComponent<RectTransform>();
    textTransform->sizeScale = {1, 1};
    textTransform->pivot = {0};
    textTransform->sizeOffset = {-5, 0};
    textTransform->positionOffset = {5, 0};
    textTransform->positionScale = {0};

    UIText* tabText = textObject->AddComponent<UIText>();
    tabText->material = Material("UI_TextBody");
    tabText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    tabText->text = name;
    tabText->verticalAlignment = UIAlignmentVertical::Center;
    tabText->horizontalAlignment = UIAlignmentHorizontal::Left;
    tabText->fontSize = 16;
    tabText->fontWeight = 0.1f; 

    return branch;
}

void HierarchyPanel::UpdateObjectHeiarchy(Object* object, EngineObject* parent, float indent)
{
    if (!object) return;

    EngineObject* branch = CreateHeiarchyBranch(object, parent, indent);

    for(Object* child : object->children)
    {
        UpdateObjectHeiarchy(child, parent, indent + 20); 
    }
}

void HierarchyPanel::UpdateSelectionVisuals()
{
    for (auto& pair : hierarchyNodeImages)
    {
        Object* obj = pair.first;
        UIImage* img = pair.second;

        if (!obj || !img) continue;

        bool isSelected = std::find(Engine::selectedObjects.begin(), Engine::selectedObjects.end(), obj) != Engine::selectedObjects.end();
        
        if (isSelected)
        {
            img->material = Material("UI_ButtonPress");
        }
        else
        {
            img->material = Material("UI_PanelBackground");
        }
    }
}

void HierarchyPanel::RefreshHierarchy()
{
    if (!sceneRootNode) return;

    hierarchyNodeImages.clear();
    WorkspaceManager::GetPanel("Hierarchy")->content->ClearChildren();

    for(Object* object : Engine::objects)
    {
        if(object->parent != nullptr)
            continue;

        UpdateObjectHeiarchy(object, WorkspaceManager::GetPanel("Hierarchy")->content, 26);
    }
}