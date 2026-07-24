#pragma once

#include "Component.h"
#include <iostream>
#include <vector>
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"
#include "MaterialData.h"
#include <glm/gtc/matrix_transform.hpp>
#include "MeshData.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Object.h"

class Mesh : public Component
{
public:
    std::shared_ptr<Material> material;
    string modelFilePath = "";
    std::shared_ptr<MeshData> meshData;

    Mesh() = default;
    Mesh(string filePath);

    void Start() override;
    void Update(float delta) override;

    glm::mat4 GetModelMatrix();
    void LoadMesh();

protected:
    string lastModelFilePath = "";
    Transform* transform;
};