#pragma once

#include "MathTypes.h"
#include "ResourceManager.h"
#include "MaterialData.h"

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
    };

public:
    inline static std::unordered_map<std::string, std::shared_ptr<Material>> engineUIMaterials;

    static void SetupMaterials()
    {
        for(const ColorData& data : colors)
        {
            if(data.name.find("Text") != std::string::npos)
            {
                std::shared_ptr<Material> mat = ResourceManager::CreateResource<Material>(data.name, "assets/Shaders/Vertex/UIShader.vert", "assets/Shaders/Fragment/TextShader.frag");
                mat->SetProperty("baseColor", data.col);
                engineUIMaterials[data.name] = mat;
            }
            else
            {
                std::shared_ptr<Material> mat = ResourceManager::CreateResource<Material>(data.name, "assets/Shaders/Vertex/UIShader.vert", "assets/Shaders/Fragment/UIShader.frag");
                mat->SetProperty("baseColor", data.col);
                engineUIMaterials[data.name] = mat;
            }
        }
    }
};

class EngineUI
{
public:
    EngineUI();
};