#include "MaterialData.h"
#include "Engine.h"

void Material::Impl::ApplyMaterial()
{
    if(!shader)
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

    int i = 0;
    for(std::string textureName : shader->requiredTextures)
    {
        unsigned int textureToUse = Engine::whiteTextureId;
        if(textures.contains(textureName))
        {
            textureToUse = textures[textureName]->textureId;
        }
        
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureToUse);
        shader->SetInt(textureName, i++);
    }
}