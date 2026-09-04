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
    RenderUIImage 
};

struct RenderCommand {
    CommandType type;
    std::shared_ptr<MeshData> mesh;
    glm::mat4 transform;
    std::shared_ptr<Material> material;

    // --- UI Specific Fields ---
    std::shared_ptr<Texture> texture = nullptr;
    std::shared_ptr<Font> font = nullptr;
    float fontWeight;
    int zOrder = 0;
    Vector2 minClipBounds = Vector2(0.0f, 0.0f);
    Vector2 maxClipBounds = Vector2(0.0f, 0.0f);
    
    std::string name;
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
    static float lastWeight;

    static void Render();

private:
    static void SortQueue();
    static void CollectUIHierarchy(Object* obj, std::vector<RenderCommand>& renderQueue, int zOrder, Vector2 currentMinClip, Vector2 currentMaxClip);
    static void RenderMesh(RenderCommand command);
    static void RenderStaticMesh(RenderCommand command);
    static void RenderUI(RenderCommand command);
    static void RenderUIText(RenderCommand command);
};