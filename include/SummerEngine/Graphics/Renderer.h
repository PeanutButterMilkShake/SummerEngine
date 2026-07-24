#pragma once

#include "MeshData.h"
#include <glm/gtc/matrix_transform.hpp>
#include "MaterialData.h"
#include <map>
#include <memory>
#include "Object.h"
#include "Transform.h"
#include "Light.h"
#include "Mesh.h"
#include "UIImage.h"

enum CommandType
{
    RenderMesh, // 0
    RenderUI,   // 1
};

struct RenderCommand
{
    CommandType type;

    std::shared_ptr<MeshData> mesh;
    std::shared_ptr<Material> material;
    glm::mat4 transform;
};

class Renderer
{
    public:
    static std::vector<RenderCommand> renderQueue;
    static glm::mat4 viewProjectionMatrix;
    static glm::mat4 orthographicMatrix;

    static void Render();

    private:
    static void SortQueue();
    static void RenderMesh(RenderCommand command);
    static void RenderStaticMesh(RenderCommand command);
    static void RenderUI(RenderCommand command);
};