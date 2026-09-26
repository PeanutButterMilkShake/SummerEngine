#include "AssetBrowser.h"
#include "Engine.h"
#include "WorkspaceManager.h"
#include "PropertyUIRegistry.h"
#include <filesystem>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace fileSystem = std::filesystem;

AssetBrowser* AssetBrowser::instance = nullptr;
std::string AssetBrowser::assetsPath = "";

AssetBrowser::AssetBrowser()
{
    instance = this;

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    assetsPath = (std::filesystem::path(exePath).parent_path() / "assets").string();

    currentPath = assetsPath;
}

void AssetBrowser::Init()
{
    fileIcon = Texture("Assets/Textures/FileBase.png");
    folderIcon = Texture("Assets/Textures/FolderIcon.png");

    assetPanel = WorkspaceManager::RegisterPanel("Asset Browser", "Editor", PanelLocation::Bottom, "Assets/Textures/Inspector.png", true);
    assetPanel->content->SetParent(WorkspaceManager::panels[PanelLocation::Bottom]);
    assetPanel->content->enabled = true;

    UIListLayout* layout = assetPanel->content->GetComponent<UIListLayout>();
    layout->layoutAxis = UIAxis::Horizontal;
    layout->horizontalAlignment = UIAlignmentHorizontal::Left;
    layout->paddingOffset = {16};

    BuildAssets();
}

void AssetBrowser::Update()
{
    if(rebuild)
    {
        BuildAssets();
    }

    if(refresh)
    {
        UpdateAssets();
    }
}

std::vector<Asset> AssetBrowser::SearchFolder(std::string path)
{
    std::vector<Asset> assets;

    for (const auto& entry : fileSystem::directory_iterator(path))
    {
        Asset asset;
        asset.name = entry.path().filename().string();
        asset.displayName = asset.name.substr(0, asset.name.find_last_of("."));
        asset.path = entry.path().string();

        if (entry.is_directory())
        {
            asset.type = AssetType::Folder;
        }
        else if (entry.is_regular_file())
        {
            asset.type = AssetType::Resource;
        }

        assets.push_back(asset);
    }

    return assets;
} 

EngineObject* AssetBrowser::BuildAssetButton(Asset& asset)
{
    Texture texture = fileIcon;
    if(asset.type == AssetType::Folder)
    {
        texture = folderIcon;
    }

    EngineObject* assetObject = new EngineObject();
    
    RectTransform* assetTransform = assetObject->AddComponent<RectTransform>();
    assetTransform->sizeOffset = {64};
    assetTransform->pivot = {0};
    
    UIImage* assetIcon = assetObject->AddComponent<UIImage>();
    assetIcon->material = Material("UI_Image");
    assetIcon->texture = texture;

    UIImage* background = assetObject->AddComponent<UIImage>();
    background->material = Material("UI_PanelBackground");

    UIButton* assetButton = assetObject->AddComponent<UIButton>();
    
    // Capture pointer to persistent memory
    Asset* assetPtr = &asset;
    assetButton->OnMouse0PressEvent.AddListener([this, assetPtr, background]()
    {
        if(this->selectedAsset != assetPtr)
        {
            this->selectedAsset = assetPtr;
            refresh = true;
        }
        else if(assetPtr->type == AssetType::Folder)
        {
            if(Time::GetTime() - assetPtr->lastClickedTime < doubleClickTime)
            {
                this->currentPath = assetPtr->path;
                this->selectedAsset = nullptr;
                this->rebuild = true;
            }
        }

        assetPtr->lastClickedTime = Time::GetTime();
    });

    EngineObject* nameObject = new EngineObject();
    nameObject->name = "Name";
    nameObject->SetParent(assetObject);

    RectTransform* nameTransform = nameObject->AddComponent<RectTransform>();
    nameTransform->sizeScale = {1,0};
    nameTransform->sizeOffset = {0,25};
    nameTransform->positionScale = {0,1};
    nameTransform->pivot = {0,0};

    UIText* nameText = nameObject->AddComponent<UIText>();
    nameText->material = Material("UI_TextBody");
    nameText->fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    nameText->fontSize = 17;
    nameText->text = asset.displayName;
    nameText->horizontalAlignment = UIAlignmentHorizontal::Center;
    nameText->verticalAlignment = UIAlignmentVertical::Top;

    return assetObject;
}

void AssetBrowser::UpdateAssets()
{
    for(Asset asset : currentAssets)
    {
        EngineObject* assetObject = asset.object;

        EngineObject* nameObj = assetObject->GetChildWithName("Name");
        if(!nameObj) continue;

        UIText* nameText = nameObj->GetComponent<UIText>();
        if(!nameText) continue;

        bool isSelected = (selectedAsset != nullptr) && (nameText->text == selectedAsset->name);

        for(UIImage* image : assetObject->GetComponentsOfType<UIImage>())
        {
            // Background panel check (images without direct texture assigned)
            if(!image->texture)
            {
                image->material = isSelected ? Material("UI_ButtonHighlight") : Material("UI_PanelBackground");
            }
        }
    }

    refresh = false;
}

void AssetBrowser::BuildAssets()
{
    assetPanel->content->ClearChildren();

    currentAssets = SearchFolder(currentPath);
 
    for(Asset& asset : currentAssets)
    {
        EngineObject* assetObject = BuildAssetButton(asset);
        assetObject->SetParent(assetPanel->content);

        asset.object = assetObject;
    }

    rebuild = false;
}