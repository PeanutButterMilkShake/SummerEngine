#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <cassert>
#include "MathTypes.h"
#include "ShaderData.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "TextureData.h"
#include <algorithm>

using MaterialProperty = std::variant<int, float, Vector3, Vector2>;

class Material : public Resource
{
public:
    std::unordered_map<std::string, MaterialProperty> materialProperties;
    std::shared_ptr<Shader> shader;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

    Material();
    Material(std::shared_ptr<Shader> _shader);
    Material(std::string vertPath, std::string fragPath);

    template <typename T>
    void SetProperty(const std::string& name, const T& value)
    {
        if constexpr (std::is_convertible_v<T, std::shared_ptr<Texture>>) 
        {
            if (value != nullptr) 
            {
                textures[name] = value;
            }
        }
        else 
        {
            materialProperties[name] = value; 
        }
    }

    void ApplyMaterial();
};