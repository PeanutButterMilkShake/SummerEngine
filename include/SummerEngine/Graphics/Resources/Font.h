#pragma once

#include "stb_image.h"
#include <vector>
#include <string>
#include "glad/glad.h"
#include "Resource.h"
#include <fstream>
#include "stb_truetype.h"
#include "ResourceManager.h"
#include <cassert>
#include <unordered_map>
#include "TextureData.h"

struct Font
{
public:
    struct GlyphMetrics {
        float uvX0, uvY0, uvX1, uvY1;
        float xoff, yoff, xoff2, yoff2;
        float xadvance;
    };

    struct Impl : public Resource
    {
        std::string fontFilePath;
        Texture fontAtlasTexture; // Clean handle without std::shared_ptr!
        std::unordered_map<int, GlyphMetrics> glyphMetrics;
        int baseResolution = 64;

        Impl(const std::string& filePath, int fontSize = 64)
        {
            LoadSDFAtlas(filePath, fontSize);
        }

        void LoadSDFAtlas(const std::string& filePath, int fontSize)
        {
            fontFilePath = filePath;
            baseResolution = 64;

            std::ifstream inputFileStream(filePath, std::ios::binary);
            assert(inputFileStream.is_open() && "Failed to open font file!");

            inputFileStream.seekg(0, std::ios::end);
            std::streamsize fileSize = inputFileStream.tellg();
            inputFileStream.seekg(0, std::ios::beg);

            std::vector<uint8_t> fontDataBuf(fileSize);
            inputFileStream.read((char*)fontDataBuf.data(), fileSize);

            stbtt_fontinfo fontInfo;
            assert(stbtt_InitFont(&fontInfo, fontDataBuf.data(), 0) && "Failed to init font info!");

            float scale = stbtt_ScaleForPixelHeight(&fontInfo, static_cast<float>(64));

            int atlasWidth = 1024;
            int atlasHeight = 1024;
            std::vector<uint8_t> atlasPixels(atlasWidth * atlasHeight, 0);

            int cursorX = 0;
            int cursorY = 0;
            int rowHeight = 0;
            
            int padding = 8;
            unsigned char onedgeValue = 180;
            float pixelDistScale = 24.0f;

            const int firstChar = 32;
            const int numChars = 95;

            for (int i = 0; i < numChars; ++i) {
                int codepoint = firstChar + i;
                
                int advance, lsb;
                stbtt_GetCodepointHMetrics(&fontInfo, codepoint, &advance, &lsb);
                
                GlyphMetrics gm = {};
                gm.xadvance = advance * scale;

                int w, h, xoff, yoff;
                unsigned char* sdfBitmap = stbtt_GetCodepointSDF(
                    &fontInfo, scale, codepoint, padding, onedgeValue, pixelDistScale, &w, &h, &xoff, &yoff
                );

                if (sdfBitmap) {
                    if (cursorX + w + 1 > atlasWidth) {
                        cursorX = 0;
                        cursorY += rowHeight + 4;
                        rowHeight = 0;
                    }

                    if (cursorY + h + 1 > atlasHeight) break;

                    for (int y = 0; y < h; ++y) {
                        for (int x = 0; x < w; ++x) {
                            atlasPixels[(cursorY + y) * atlasWidth + (cursorX + x)] = sdfBitmap[y * w + x];
                        }
                    }

                    float halfTexelX = 0.5f / atlasWidth;
                    float halfTexelY = 0.5f / atlasHeight;

                    gm.uvX0 = (static_cast<float>(cursorX) / atlasWidth) + halfTexelX;
                    gm.uvY0 = (static_cast<float>(cursorY) / atlasHeight) + halfTexelY;
                    gm.uvX1 = (static_cast<float>(cursorX + w) / atlasWidth) - halfTexelX;
                    gm.uvY1 = (static_cast<float>(cursorY + h) / atlasHeight) - halfTexelY;
                    
                    gm.xoff = static_cast<float>(xoff);
                    gm.yoff = static_cast<float>(yoff);
                    gm.xoff2 = static_cast<float>(xoff + w);
                    gm.yoff2 = static_cast<float>(yoff + h);

                    if (h > rowHeight) rowHeight = h;
                    cursorX += w + 4;

                    stbtt_FreeSDF(sdfBitmap, nullptr);
                }

                glyphMetrics[codepoint] = gm;
            }

            std::string textureKey = filePath + "_SDFMasterAtlas";
            fontAtlasTexture = Texture(textureKey, atlasWidth, atlasHeight, atlasPixels.data());
        }
    };

    std::shared_ptr<Impl> m_impl;

    Font(std::nullptr_t) : m_impl(nullptr) {}
    Font() = default;

    Font(const std::string& filePath, int fontSize = 64)
    {
        std::string key = filePath + "_" + std::to_string(fontSize);
        m_impl = ResourceManager::GetResource<Impl>(key);
        if (m_impl) return;

        m_impl = std::make_shared<Impl>(filePath, fontSize);
        ResourceManager::AddToCache<Impl>(key, m_impl);
    }

    // equality check
    bool operator==(const Font& other) const { return m_impl == other.m_impl; }
    bool operator!=(const Font& other) const { return m_impl != other.m_impl; }

    bool operator==(std::nullptr_t) const { return m_impl == nullptr; }
    bool operator!=(std::nullptr_t) const { return m_impl != nullptr; }

    bool operator<(const Font& other) const { return m_impl.get() < other.m_impl.get(); }
    bool operator>(const Font& other) const { return m_impl.get() > other.m_impl.get(); }

    Impl* operator->() { return m_impl.get(); }
    const Impl* operator->() const { return m_impl.get(); }
    explicit operator bool() const { return m_impl != nullptr; }
};