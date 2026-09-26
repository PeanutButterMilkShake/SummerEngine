#pragma once

#include "stb_image.h"
#include <vector>
#include <string>
#include "glad/glad.h"
#include "Resource.h"
#include "ResourceManager.h"
#include <memory>

enum WrapMode
{
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    EdgeClamp = GL_CLAMP_TO_EDGE,
    BorderClamp = GL_CLAMP_TO_BORDER,
};

enum FilterMode
{
    Bilinear = GL_LINEAR,
    Nearest = GL_NEAREST,
};

struct Texture
{
public:
    struct Impl : public Resource
    {
        WrapMode wrapMode = WrapMode::Repeat;
        FilterMode filterMode = FilterMode::Bilinear;

        int width = 0, height = 0, channels = 0;
        unsigned int textureId = 0;
        std::string name;

        Impl(const std::string& filePath)
        {
            unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

            if (!data) 
            { 
                printf("Failed to load texture: %s\n", filePath.c_str()); 
                return;
            }

            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

            stbi_image_free(data);
        }

        Impl(int width, int height, const uint8_t* bitmap) : width(width), height(height)
        {
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

            wrapMode = WrapMode::EdgeClamp;

            GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_RED };
            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        }

        ~Impl()
        {
            if (textureId != 0) glDeleteTextures(1, &textureId);
        }

        void Use() const
        {
            if (textureId != 0) glBindTexture(GL_TEXTURE_2D, textureId);
        }
    };

    std::shared_ptr<Impl> m_impl;

    Texture(std::nullptr_t) : m_impl(nullptr) {}
    Texture() = default;

    // Load from disk (caches automatically using filePath as key)
    Texture(const std::string& filePath)
    {
        m_impl = ResourceManager::GetResource<Impl>(filePath);
        if (m_impl) return;

        m_impl = std::make_shared<Impl>(filePath);
        ResourceManager::AddToCache<Impl>(filePath, m_impl);
    }

    // Load from raw bitmap (caches using key parameter)
    Texture(const std::string& key, int width, int height, const uint8_t* bitmap)
    {
        m_impl = ResourceManager::GetResource<Impl>(key);
        if (m_impl) return;

        m_impl = std::make_shared<Impl>(width, height, bitmap);
        ResourceManager::AddToCache<Impl>(key, m_impl);
    }

    void Use() const { if (m_impl) m_impl->Use(); }

    // equality check
    bool operator==(const Texture& other) const { return m_impl == other.m_impl; }
    bool operator!=(const Texture& other) const { return m_impl != other.m_impl; }

    bool operator==(std::nullptr_t) const { return m_impl == nullptr; }
    bool operator!=(std::nullptr_t) const { return m_impl != nullptr; }

    bool operator<(const Texture& other) const { return m_impl.get() < other.m_impl.get(); }
    bool operator>(const Texture& other) const { return m_impl.get() > other.m_impl.get(); }

    Impl* operator->() { return m_impl.get(); }
    const Impl* operator->() const { return m_impl.get(); }
    explicit operator bool() const { return m_impl != nullptr; }
};