#include "MaterialData.h"

Material::Material()
{

}

Material::Material(std::shared_ptr<ShaderData> _shader)
{
    shader = _shader;
}

Material::Material(std::string vertPath, std::string fragPath)
{
    shader = ResourceManager::Get<ShaderData>(vertPath+fragPath, vertPath, fragPath);
}

void Material::ApplyMaterial()
{
    if(shader == nullptr)
        assert("Please assign a shader before applying material");

    for(auto [name, property] : materialProperties)
    {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<T, int>)
                shader->SetInt(name, arg);
            else if constexpr (std::is_same_v<T, float>)
                shader->SetFloat(name, arg);
            else if constexpr (std::is_same_v<T, Vector2>)
                shader->SetVector2(name, arg);
            else if constexpr (std::is_same_v<T, Vector3>)
                shader->SetVector3(name, arg);

        }, property);
    }
}