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
    std::shared_ptr<Material> material;
    std::shared_ptr<Texture> texture;   // per-instance texture, independent of the (possibly shared) material
    std::shared_ptr<MeshData> meshData;

    void Start() override;
};