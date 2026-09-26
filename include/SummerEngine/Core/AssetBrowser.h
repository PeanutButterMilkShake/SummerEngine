#pragma once

#include "PanelContent.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "UIText.h"
#include "EnginePanel.h"
#include "SETime.h"

class Object;
class EngineObject;
class UIImage;

enum AssetType
{
    Folder,
    Resource
};

struct Asset
{
    std::string name;
    std::string displayName;
    AssetType type;
    std::string path;
    EngineObject* object;
    float lastClickedTime = 0;
};

class AssetBrowser : public PanelContent
{
public:
    static AssetBrowser* instance;
    static std::string assetsPath;

    float doubleClickTime = 0.2f;

    AssetBrowser();
    
    void Init() override;
    void Update() override;

    std::vector<Asset> SearchFolder(std::string);
    EngineObject* BuildAssetButton(Asset& asset);
    void BuildAssets();
    void UpdateAssets();

private:
    Texture fileIcon;
    Texture folderIcon;
    bool refresh = false;
    bool rebuild = false;
    EnginePanel* assetPanel;
    std::string currentPath;
    Asset* selectedAsset;
    std::vector<Asset> currentAssets;
};