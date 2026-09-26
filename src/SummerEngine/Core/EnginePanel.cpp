#include "EnginePanel.h"
#include "EngineUI.h"

EnginePanel::EnginePanel(std::string _title, std::string _category, Vector2 _defaultLocation, std::string iconPath) : title(_title), category(_category), defaultLocation(_defaultLocation)
{
    // Create title tab
    RectTransform* tabTransform = tabObject->AddComponent<RectTransform>();
    tabTransform->pivot = {0,0};
    tabTransform->sizeOffset = {140, 25};
    tabObject->AddComponent<UIImage>()->material = Material("UI_PanelForeground");
    tabObject->AddComponent<UIButton>();
    tabObject->name = title + " tab";

    // Create title Icon
    EngineObject* tabIconObject = new EngineObject();
    tabIconObject->SetParent(tabObject);

    RectTransform* iconTransform = tabIconObject->AddComponent<RectTransform>();
    iconTransform->sizeOffset = {18};
    iconTransform->positionOffset = {5,3};
    iconTransform->pivot = {0,0};

    UIImage* icon = tabIconObject->AddComponent<UIImage>();
    icon->material = Material("Image");
    icon->texture = Texture(iconPath);

    // Create title Text
    EngineObject* tabTextObject = new EngineObject();
    tabTextObject->SetParent(tabObject);

    RectTransform* textTransform = tabTextObject->AddComponent<RectTransform>();
    textTransform->sizeScale = {1,1};
    textTransform->pivot = {.5};
    textTransform->sizeOffset = {0,0};
    textTransform->positionOffset = {27,0};
    textTransform->positionScale = {.5,.5};

    UIText* tabText = tabTextObject->AddComponent<UIText>();
    tabText->material = Material("UI_TextBody");
    tabText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    tabText->text = title;
    tabText->verticalAlignment = UIAlignmentVertical::Center;
    tabText->horizontalAlignment = UIAlignmentHorizontal::Left;
    tabText->fontSize = 16;

    tabObject->enabled = false;

    // Create content object
    RectTransform* contentTransform = content->AddComponent<RectTransform>();
    contentTransform->sizeOffset = {0,-25};
    contentTransform->sizeScale = {1,1};
    contentTransform->positionOffset = {0,25};
    contentTransform->pivot = {0,0};
    
    UIListLayout* contentLayout = content->AddComponent<UIListLayout>();
    contentLayout->layoutAxis = UIAxis::Vertical;
    contentLayout->horizontalAlignment = UIAlignmentHorizontal::Center;
    contentLayout->paddingOffset = {0,3};

    UIScrollView* contentScrollView = content->AddComponent<UIScrollView>();
    contentScrollView->scrollDirection = UIAxis::Vertical;
    contentScrollView->scrollSpeed = 5;
    contentScrollView->scrollViewSize = {0,-2};

    content->name = title + " Content";
    content->enabled = false;
    content->GetComponent<RectTransform>()->clipChildren = true;
}