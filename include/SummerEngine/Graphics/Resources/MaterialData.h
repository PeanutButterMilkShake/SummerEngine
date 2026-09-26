#pragma once

#include <unordered_map>
#include <variant>
#include <string>
#include <cassert>
#include "MathTypes.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "TextureData.h"
#include "ShaderData.h"
#include <algorithm>
#include <memory>

using MaterialProperty = std::variant<int, float, Vector3, Vector2>;

class Material
{
public:
    struct Impl : public Resource
    {
        std::unordered_map<std::string, MaterialProperty> materialProperties;
        Shader shader; 
        std::unordered_map<std::string, Texture> textures;

        Impl() = default;
        Impl(Shader _shader) : shader(_shader) {}
        Impl(const std::string& shaderPath) : shader(Shader(shaderPath)) {}

        template <typename T>
        void SetProperty(const std::string& name, const T& value)
        {
            if constexpr (std::is_same_v<T, Texture>) 
            {
                textures[name] = value;
            }
            else 
            {
                materialProperties[name] = value; 
            }
        }

        void ApplyMaterial();
    };

    std::shared_ptr<Impl> m_impl;

    Material(std::nullptr_t) : m_impl(nullptr) {}

    Material() = default;

    Material(std::shared_ptr<Impl> impl) : m_impl(impl) {}

    Material(const std::string& materialKey)
    {
        m_impl = ResourceManager::GetResource<Impl>(materialKey);
    }

    Material(const std::string& materialKey, Shader shader)
    {
        m_impl = ResourceManager::GetResource<Impl>(materialKey);
        if (m_impl) return;

        m_impl = std::make_shared<Impl>(shader);
        ResourceManager::AddToCache<Impl>(materialKey, m_impl);
    }

    template <typename T>
    void SetProperty(const std::string& name, const T& value)
    {
        if (m_impl) m_impl->SetProperty(name, value);
    }

    void ApplyMaterial() { if (m_impl) m_impl->ApplyMaterial(); }

    // equality check
    bool operator==(const Material& other) const { return m_impl == other.m_impl; }
    bool operator!=(const Material& other) const { return m_impl != other.m_impl; }

    bool operator==(std::nullptr_t) const { return m_impl == nullptr; }
    bool operator!=(std::nullptr_t) const { return m_impl != nullptr; }

    bool operator<(const Material& other) const { return m_impl.get() < other.m_impl.get(); }
    bool operator>(const Material& other) const { return m_impl.get() > other.m_impl.get(); }

    Impl* operator->() { return m_impl.get(); }
    const Impl* operator->() const { return m_impl.get(); }
    explicit operator bool() const { return m_impl != nullptr; }
};