#include "InspectorPanel.h"
#include "Engine.h"
#include "WorkspaceManager.h"
#include "PropertyUIRegistry.h"

InspectorPanel* InspectorPanel::instance = nullptr;

InspectorPanel::InspectorPanel()
{
    instance = this;
}

void InspectorPanel::Init()
{
    inspectorPanel = WorkspaceManager::RegisterPanel("Inspector", "Editor", PanelLocation::Right, "Assets/Textures/Inspector.png", true);
    inspectorPanel->content->SetParent(WorkspaceManager::panels[PanelLocation::Right]);
    inspectorPanel->content->enabled = true;

    inspectorPanel->content->GetComponent<RectTransform>()->sizeOffset = {0,-50};
    inspectorPanel->content->GetComponent<RectTransform>()->positionOffset = {0,65};
    inspectorPanel->content->GetComponent<UIListLayout>()->paddingOffset = {0,5};

    EngineObject* tabRibbon = new EngineObject();
    tabRibbon->name = "Ribbon";
    tabRibbon->SetParent(inspectorPanel->content->parent);

    RectTransform* ribbonTransform = tabRibbon->AddComponent<RectTransform>();
    ribbonTransform->sizeScale = {1, 0};
    ribbonTransform->sizeOffset = {0, 25};
    ribbonTransform->positionOffset = {0, 25};
    ribbonTransform->pivot = {0};
    ribbonTransform->clipChildren = true;
    ribbonTransform->zOffset = -3;

    UIImage* ribbon = tabRibbon->AddComponent<UIImage>();
    ribbon->material = EngineUIColors::engineUIMaterials["PanelForeground"];

    EngineObject* nameTextObject = new EngineObject();
    nameTextObject->SetParent(tabRibbon);

    RectTransform* nameTextTransform = nameTextObject->AddComponent<RectTransform>();
    nameTextTransform->pivot = {0,0};
    nameTextTransform->sizeScale = {1, 1};
    nameTextTransform->sizeOffset = {0, 0};
    nameTextTransform->positionOffset = {32, 0};

    nameText = nameTextObject->AddComponent<UIText>();
    nameText->material = EngineUIColors::engineUIMaterials["TextBody"];
    nameText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    nameText->text = "";
    nameText->verticalAlignment = UIAlignmentVertical::Center;
    nameText->horizontalAlignment = UIAlignmentHorizontal::Left;
    nameText->fontSize = 20;
}

std::type_index GetTypeIndex(PropertyType type)
{
    switch (type)
    {
        case PropertyType::Float: return typeid(float);
        case PropertyType::Int: return typeid(int);
        case PropertyType::String: return typeid(std::string);
        case PropertyType::Bool: return typeid(bool);
        case PropertyType::Vector2: return typeid(Vector2);
        case PropertyType::Vector3: return typeid(Vector3);
    }
    return typeid(void);
}

void InspectorPanel::Update()
{
    if(lastSelectedObjects != Engine::selectedObjects)
    {
        inspectorPanel->content->ClearChildren();
        for(Object* object : Engine::selectedObjects)
        {
            nameText->text = object->name;

            for(std::pair<std::type_index, std::vector<PropertyInfo>> componentData : object->GetComponentData())
            {
                std::type_index componentType = componentData.first;
                string componentName = componentType.name();
                
                for(PropertyInfo property : componentData.second)
                {
                    EngineObject* propertyUI = PropertyRegistry::BuildUI(GetTypeIndex(property.type), property.name, property.get(), property.set);
                    if(propertyUI == nullptr)
                    {
                        printf("Property UI is null \n");
                        continue;
                    }

                    propertyUI->SetParent(inspectorPanel->content);
                }
            }
        }        

        lastSelectedObjects = Engine::selectedObjects;
    }
}