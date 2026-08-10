#pragma once

#include "MeshData.h"
#include <glm/gtc/matrix_transform.hpp>
#include "MaterialData.h"
#include <map>
#include <memory>
#include "Object.h"
#include "Transform.h"
#include "Light.h"
#include "UIText.h"
#include "Mesh.h"
#include "UIImage.h"
#include "Font.h"

enum class CommandType
{
    RenderMesh,
    RenderUI,
    RenderUIText,
    RenderUIImage // New verbose command type for textured UI images
};

struct RenderCommand {
    CommandType type;
    std::shared_ptr<MeshData> mesh;
    glm::mat4 transform;
    std::shared_ptr<Material> material;
    std::shared_ptr<Font> font;
    std::shared_ptr<Texture> texture;   // bound texture: UIImage::texture, or the font's atlas for text
    int zOrder = 0;                     // draw layer (root panel index) - preserved during UI sort
};

class Renderer
{
public:
    static std::vector<RenderCommand> renderQueue;
    static glm::mat4 viewProjectionMatrix;
    static glm::mat4 orthographicMatrix;
    static int lastShader;
    static shared_ptr<MeshData> lastMesh;
    static shared_ptr<Material> lastMaterial;
    static std::shared_ptr<Texture> lastTexture;

    static void Render();

private:
    static void SortQueue();
    static void CollectUIHierarchy(Object* obj, std::vector<RenderCommand>& renderQueue, int zOrder);
    static void RenderMesh(RenderCommand command);
    static void RenderStaticMesh(RenderCommand command);
    static void RenderUI(RenderCommand command);
    static void RenderUIText(RenderCommand command);
};