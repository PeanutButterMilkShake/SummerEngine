#pragma once

#include "RectTransform.h"
#include "MaterialData.h"
#include "Component.h"
#include "MeshData.h"
#include "ResourceManager.h"
#include "Object.h"
#include "Engine.h"

class UIImage : public Component
{
public:
    RectTransform* transform;
    shared_ptr<Material> material;
    shared_ptr<MeshData> meshData;

    void Start() override;

    Vector2 referenceResolution = Vector2(960, 600);

    glm::mat4 GetRectMatrix();

private:
};