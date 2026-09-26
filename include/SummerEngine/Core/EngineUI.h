#pragma once

#include "MathTypes.h"
#include "ResourceManager.h"
#include "MaterialData.h"
#include "PanelContent.h"
#include <unordered_map>
#include <string>
#include <memory>

class Object;
class EngineObject;
class EnginePanel;
class UIImage;

struct EngineUIColors
{
private:
    struct ColorData
    {
        Color3 col;
        std::string name;

        ColorData(std::string _name, Color3 _col)
        {
            name = _name; col = _col;
        }
    };

    inline static const ColorData colors[] = {
        ColorData("Image", Color3(1,1,1)),
        ColorData("PanelBackground", Color3(0.09412f, 0.09412f, 0.14510f)),
        ColorData("PanelForeground", Color3(0.11765f, 0.11765f, 0.18039f)),
        ColorData("PanelRibbon", Color3(0.06667f, 0.06667f, 0.10588f)),
        ColorData("TextBody", Color3(0.80392f, 0.83922f, 0.95686f)),
        ColorData("ButtonHighlight", Color3(0.19215f, 0.19607f, 0.26666f)),
        ColorData("ButtonPress", Color3(0.27058f, 0.27843f, 0.35294f)),

        ColorData("N/A", Color3(1,0,1)),
    };

public:
    static void SetupMaterials()
    {
        for(const ColorData& data : colors)
        {
            if(data.name.find("Text") != std::string::npos)
            {
                std::shared_ptr<Material> mat = std::make_shared<Material>("assets/Shaders/TextShader.glsl");
                mat->SetProperty("baseColor", data.col);

                ResourceManager::AddToCache("UI_" + data.name, mat);
            }
            else
            {
                std::shared_ptr<Material> mat = std::make_shared<Material>("assets/Shaders/UIShader.glsl");
                mat->SetProperty("baseColor", data.col);

                ResourceManager::AddToCache("UI_" + data.name, mat);
            }
        }
    }
};

class EngineUI
{
public:
    static EngineUI* singleton;

    std::vector<PanelContent*> panels;

    EngineUI();
    void Update();
    
private:
};