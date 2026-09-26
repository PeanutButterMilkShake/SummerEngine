#include "UIText.h"
#include "ResourceManager.h"
#include "RectTransform.h"
#include "MeshData.h"
#include <algorithm>
#include <limits>

UIText::UIText() {}

void UIText::Start()
{
    transform = GetComponent<RectTransform>();

    lastText = text;
    lastFontPath = fontFilePath;
    lastCharPadding = characterPadding;
    lastWordPadding = wordPadding;
    lastLinePadding = linePadding;
    lastHAlign = horizontalAlignment;
    lastVAlign = verticalAlignment;

    lastBoxSize = transform ? transform->GetAbsoluteSize() : Vector2(-1.0f, -1.0f);

    font = Font(fontFilePath);
    GenerateTextMesh();
}

void UIText::Update(float delta)
{
    Vector2 currentBoxSize = transform ? transform->GetAbsoluteSize() : Vector2(9999.0f, 30.0f);

    bool needsFontReload = fontFilePath != lastFontPath;
    bool boxResized = (std::abs(currentBoxSize.x - lastBoxSize.x) > 0.01f) ||
                    (std::abs(currentBoxSize.y - lastBoxSize.y) > 0.01f);

    bool needsMeshRegen = (text != lastText) || needsFontReload ||
                        (characterPadding != lastCharPadding) || (wordPadding != lastWordPadding) ||
                        (linePadding != lastLinePadding) ||
                        (horizontalAlignment != lastHAlign) || (verticalAlignment != lastVAlign) ||
                        boxResized;

    if (needsFontReload)
    {
        lastFontPath = fontFilePath;
        font = Font(fontFilePath);
    }

    if (needsMeshRegen)
    {
        lastText = text;
        lastCharPadding = characterPadding;
        lastWordPadding = wordPadding;
        lastLinePadding = linePadding;
        lastHAlign = horizontalAlignment;
        lastVAlign = verticalAlignment;
        
        GenerateTextMesh();
    }
}

void UIText::GenerateTextMesh()
{
    if (!font || text.empty()) return;

    float scale = fontSize / static_cast<float>(font->baseResolution);
    
    // UIText.cpp — GenerateTextMesh()
    float maxWidth = 9999.0f;
    float maxHeight = 30.0f; 
    if (transform) {
        glm::mat4 rectMat = transform->GetRectMatrix();
        maxWidth = glm::length(glm::vec3(rectMat[0])); 
        maxHeight = glm::length(glm::vec3(rectMat[1])); 
    }

    // FIX: Start at 0.0f instead of fontSize so the bounding box shifts properly from the top edge
    float currentX = 0.0f;
    float currentY = 0.0f; 
    float baseLineHeight = (fontSize * 1.2f) + linePadding;

    if (!meshData) {
        meshData = MeshData();
    } else {
        meshData->vertices.clear();
        meshData->uvs.clear();
        meshData->indices.clear();
        meshData->normals.clear();
    }

    unsigned int currentIndex = 0;
    size_t lineStartVertexIndex = 0;

    auto ApplyHorizontalAlignment = [&](size_t startIdx, float lineWidth) {
        float shiftX = 0.0f;
        if (horizontalAlignment == UIAlignmentHorizontal::Center) {
            shiftX = (maxWidth - lineWidth) / 2.0f;
        } else if (horizontalAlignment == UIAlignmentHorizontal::Right) {
            shiftX = maxWidth - lineWidth;
        }

        if (shiftX != 0.0f) {
            for (size_t v = startIdx; v < meshData->vertices.size(); v += 3) {
                meshData->vertices[v] += shiftX;
            }
        }
    };

    for (size_t i = 0; i < text.size(); i++)
    {
        char c = text[i];

        if (c == '\n') {
            ApplyHorizontalAlignment(lineStartVertexIndex, currentX);
            lineStartVertexIndex = meshData->vertices.size();
            currentX = 0.0f;
            currentY += baseLineHeight;
            continue;
        }

        if (i == 0 || text[i - 1] == ' ' || text[i - 1] == '\n') {
            float wordWidth = 0.0f;
            for (size_t j = i; j < text.size() && text[j] != ' ' && text[j] != '\n'; j++) {
                if (font->glyphMetrics.find(text[j]) != font->glyphMetrics.end()) {
                    wordWidth += (font->glyphMetrics.at(text[j]).xadvance * scale) + characterPadding;
                }
            }
            if (wordWidth > 0) wordWidth -= characterPadding;

            if (currentX + wordWidth > maxWidth && currentX > 0.0f) {
                ApplyHorizontalAlignment(lineStartVertexIndex, currentX);
                lineStartVertexIndex = meshData->vertices.size();
                currentX = 0.0f;
                currentY += baseLineHeight;
            }
        }

        if (font->glyphMetrics.find(c) == font->glyphMetrics.end()) continue;
        const auto& gm = font->glyphMetrics.at(c);

        if (c == ' ') {
            currentX += (gm.xadvance * scale) + wordPadding;
            continue;
        }

        float x0 = currentX + gm.xoff * scale;
        float y0 = currentY + gm.yoff * scale;
        float x1 = currentX + gm.xoff2 * scale;
        float y1 = currentY + gm.yoff2 * scale;

        meshData->vertices.push_back(x0); meshData->vertices.push_back(y0); meshData->vertices.push_back(0.0f);
        meshData->vertices.push_back(x1); meshData->vertices.push_back(y0); meshData->vertices.push_back(0.0f);
        meshData->vertices.push_back(x1); meshData->vertices.push_back(y1); meshData->vertices.push_back(0.0f);
        meshData->vertices.push_back(x0); meshData->vertices.push_back(y1); meshData->vertices.push_back(0.0f);

        meshData->uvs.push_back(gm.uvX0); meshData->uvs.push_back(gm.uvY0);
        meshData->uvs.push_back(gm.uvX1); meshData->uvs.push_back(gm.uvY0);
        meshData->uvs.push_back(gm.uvX1); meshData->uvs.push_back(gm.uvY1);
        meshData->uvs.push_back(gm.uvX0); meshData->uvs.push_back(gm.uvY1);

        meshData->indices.push_back(currentIndex + 0);
        meshData->indices.push_back(currentIndex + 1);
        meshData->indices.push_back(currentIndex + 2);
        
        meshData->indices.push_back(currentIndex + 0);
        meshData->indices.push_back(currentIndex + 2);
        meshData->indices.push_back(currentIndex + 3);

        currentIndex += 4;
        currentX += (gm.xadvance * scale) + characterPadding;
    }

ApplyHorizontalAlignment(lineStartVertexIndex, currentX);

    if (!meshData->vertices.empty())
    {
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();

        for (size_t v = 1; v < meshData->vertices.size(); v += 3) {
            float y = meshData->vertices[v];
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }

        float textHeight = maxY - minY;
        float shiftY = 0.0f;

        if (verticalAlignment == UIAlignmentVertical::Center) {
            shiftY = (maxHeight - textHeight) / 2.0f - minY;
        } else if (verticalAlignment == UIAlignmentVertical::Bottom) {
            shiftY = maxHeight - maxY;
        } else { // Top
            shiftY = -minY;
        }

        // FIX: Remove pivotOffsetX and pivotOffsetY entirely. 
        // Local (0,0) remains at the Top-Left of the text bounds.
        for (size_t v = 1; v < meshData->vertices.size(); v += 3) {
            meshData->vertices[v] += shiftY; 
        }
    }

    meshData->LoadMesh();
}