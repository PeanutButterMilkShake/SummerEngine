#pragma once

#include "RectTransform.h"
#include "MaterialData.h"
#include "Component.h"
#include "MeshData.h"
#include "ResourceManager.h"
#include "Object.h"
#include "TextureData.h"

class UIImage : public Component
{
public:
    RectTransform* transform;
    Material material;
    Texture texture;
    MeshData meshData;
    Color3 tint;

    void Start() override;
};