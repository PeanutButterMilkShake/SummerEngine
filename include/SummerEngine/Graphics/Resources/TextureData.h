#pragma once

#include "stb_image.h"

#include <vector>
#include <string>
#include "glad/glad.h"
#include "Resource.h"

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

struct Texture : public Resource
{
public:
    WrapMode wrapMode = WrapMode::Repeat;
    FilterMode filterMode = FilterMode::Bilinear;

    int width, height, channels;
    unsigned char* data;
    unsigned int textureId;
    std::string name;

    Texture(std::string filePath)
    {
        //Create image data
        data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

        if (!data) 
        { 
            printf("Failed to load texture: %s\n", filePath.c_str()); 
        }

        // Bind image data
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D); // Create LODs

        // Set filter modes 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

        // Set wrap modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

        stbi_image_free(data);
    }

    ~Texture()
    {
        glDeleteTextures(1, &textureId);
    }

    void Use()
    {
        glBindTexture(GL_TEXTURE_2D, textureId);
    }
};