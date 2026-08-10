#include "EnginePanel.h"
#include "EngineUI.h"

EnginePanel::EnginePanel(std::string _title, std::string _category, Vector2 _defaultLocation, std::string iconPath) : title(_title), category(_category), defaultLocation(_defaultLocation)
{
    // Create title tab
    RectTransform* tabTransform = tabObject->AddComponent<RectTransform>();
    tabTransform->pivot = {0,0};
    tabTransform->sizeOffset = {140, 25};
    tabObject->AddComponent<UIImage>()->material = EngineUIColors::engineUIMaterials["PanelForeground"];
    tabObject->AddComponent<UIButton>();
    tabObject->name = title + " tab";

    // Create title Icon
    Object* tabIconObject = new Object();
    tabIconObject->SetParent(tabObject);

    RectTransform* iconTransform = tabIconObject->AddComponent<RectTransform>();
    iconTransform->sizeOffset = {18};
    iconTransform->positionOffset = {5,3};
    iconTransform->pivot = {0,0};

    UIImage* icon = tabIconObject->AddComponent<UIImage>();
    icon->material = EngineUIColors::engineUIMaterials["Image"];
    icon->texture = ResourceManager::CreateResource<Texture>(title  + " tab Icon", iconPath);

    // Create title Text
    Object* tabTextObject = new Object();
    tabTextObject->SetParent(tabObject);

    RectTransform* textTransform = tabTextObject->AddComponent<RectTransform>();
    textTransform->sizeScale = {1,1};
    textTransform->pivot = {0,0};
    textTransform->sizeOffset = {0,0};
    textTransform->positionOffset = {27,0};

    UIText* tabText = tabTextObject->AddComponent<UIText>();
    tabText->material = EngineUIColors::engineUIMaterials["TextBody"];
    tabText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    tabText->text = title;
    tabText->verticalAlignment = UIAlignmentVertical::Center;
    tabText->horizontalAlignment = UIAlignmentHorizontal::Left;
    tabText->fontSize = 20;

    tabObject->enabled = false;
}