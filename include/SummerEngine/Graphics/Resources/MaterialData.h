#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <cassert>
#include "MathTypes.h"
#include "ShaderData.h"
#include "Resource.h"
#include "ResourceManager.h"

using MaterialProperty = std::variant<int, float, Vector3, Vector2>;

class Material : public Resource
{
public:
    std::unordered_map<std::string, MaterialProperty> materialProperties;
    std::shared_ptr<ShaderData> shader;

    Material();
    Material(std::shared_ptr<ShaderData> _shader);
    Material(std::string vertPath, std::string fragPath);

    template <typename T>
    void SetProperty(const std::string& name, const T& value)
    {
        materialProperties[name] = value;
    }

    void ApplyMaterial();
};