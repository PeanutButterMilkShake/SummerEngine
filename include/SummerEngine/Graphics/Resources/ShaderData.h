#pragma once

#include <string>
#include <vector>
#include "MathTypes.h"
#include "Utility.h"
#include "Resource.h"
#include "ResourceManager.h"
#include <memory>

class Shader
{
public:
    struct Impl : public Resource
    {
        unsigned int shaderId;
        std::vector<std::string> requiredTextures;

        Impl(const std::string& shaderPath);
        ~Impl();

        void Use();
        void SetInt(const std::string &name, const int &value);
        void SetFloat(const std::string &name, const float &value);
        void SetVector2(const std::string &name, const Vector2 &value);
        void SetVector3(const std::string &name, const Vector3 &value);
        void SetVector4(const std::string &name, const glm::vec4 &value);
        void SetMat4(const std::string &name, const glm::mat4 &mat);
        void InspectShaderTextures();
    };

    std::shared_ptr<Impl> m_impl;

    Shader() = default;

    Shader(std::nullptr_t) : m_impl(nullptr) {}

    Shader(const std::string& shaderPath)
    {
        m_impl = ResourceManager::GetResource<Impl>(shaderPath);
        if (m_impl) return;

        m_impl = std::make_shared<Impl>(shaderPath);
        ResourceManager::AddToCache<Impl>(shaderPath, m_impl);
    }

    void Use() const { if (m_impl) m_impl->Use(); }

    // equality check
    bool operator==(const Shader& other) const { return m_impl == other.m_impl; }
    bool operator!=(const Shader& other) const { return m_impl != other.m_impl; }

    bool operator==(std::nullptr_t) const { return m_impl == nullptr; }
    bool operator!=(std::nullptr_t) const { return m_impl != nullptr; }

    bool operator<(const Shader& other) const { return m_impl.get() < other.m_impl.get(); }
    bool operator>(const Shader& other) const { return m_impl.get() > other.m_impl.get(); }

    Impl* operator->() { return m_impl.get(); }
    const Impl* operator->() const { return m_impl.get(); }
    explicit operator bool() const { return m_impl != nullptr; }
};