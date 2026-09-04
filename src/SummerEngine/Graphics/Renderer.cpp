#include "Renderer.h"
#include "Engine.h"
#include <algorithm>
#include "EngineObject.h"

#include <GLFW/glfw3.h> 

std::vector<RenderCommand> Renderer::renderQueue;
glm::mat4 Renderer::viewProjectionMatrix;
glm::mat4 Renderer::orthographicMatrix;
int Renderer::lastShader = -1;
shared_ptr<MeshData> Renderer::lastMesh = nullptr;
shared_ptr<Material> Renderer::lastMaterial = nullptr;
shared_ptr<Texture> Renderer::lastTexture = nullptr;
float Renderer::lastWeight = 0;

void Renderer::Render()
{
    lastShader = -1;
    lastWeight = -1.0f;
    lastMesh = nullptr;
    lastMaterial = nullptr;
    lastTexture = nullptr;

    renderQueue.clear();

    // Collect meshes from scene
    for(Object* object : Engine::objects)
    {
        if(object == nullptr || !object->enabled)
            continue;

        for(Mesh* mesh : object->GetComponentsOfType<Mesh>())
        {
            if (mesh == nullptr) continue;

            RenderCommand command;
            command.type = CommandType::RenderMesh;
            command.mesh = mesh->meshData;
            command.transform = mesh->GetModelMatrix();
            command.material = mesh->material;
            command.name = object->name;

            renderQueue.push_back(command);
        }
    }

    // Collect meshes from engine
    for(EngineObject* object : Engine::engineObjects)
    {
        if(object == nullptr || !object->enabled)
            continue;

        for(Mesh* mesh : object->GetComponentsOfType<Mesh>())
        {
            if (mesh == nullptr) continue;

            RenderCommand command;
            command.type = CommandType::RenderMesh;
            command.mesh = mesh->meshData;
            command.transform = mesh->GetModelMatrix();
            command.material = mesh->material;
            command.name = object->name;

            renderQueue.push_back(command);
        }
    }

    // Collect UI with window relative clip bounds
    Vector2 screenMin = Vector2(0.0f, 0.0f);
    Vector2 screenMax = Vector2((float)Engine::windowDimensions.x, (float)Engine::windowDimensions.y);

    // Collect UI from scene
    for(Object* object : Engine::objects)
    {
        if(object == nullptr)
            continue;

        if(object->enabled && object->parent == nullptr && object->GetComponent<RectTransform>())
        {
            CollectUIHierarchy(object, renderQueue, 0, screenMin, screenMax);
        }
    }

    // Collect UI from Engine
    for(EngineObject* object : Engine::engineObjects)
    {
        if(object == nullptr)
            continue;

        if(object->enabled && object->parent == nullptr && object->GetComponent<RectTransform>())
        {
            CollectUIHierarchy(object, renderQueue, 0, screenMin, screenMax);
        }
    }

    // Sort queue by shader, material, mesh, and UI zOrder
    SortQueue();

    // Calculate view projection matrix
    viewProjectionMatrix = Engine::mainCamera->PerspectiveMatrix() * Engine::mainCamera->ViewMatrix();

    // Calculate orthographic matrix for UI
    orthographicMatrix = glm::ortho(0.f, (float)Engine::windowDimensions.x, (float)Engine::windowDimensions.y, 0.f, -1.f, 1000.f);

    for(RenderCommand& command : renderQueue)
    {
        if(command.type == CommandType::RenderMesh)
        {
            RenderMesh(command);
        }
        else if(command.type == CommandType::RenderUI || command.type == CommandType::RenderUIText)
        {
            RenderUI(command); 
        }
    }
}

void Renderer::CollectUIHierarchy(Object* obj, std::vector<RenderCommand>& renderQueue, int zOrder, Vector2 currentMinClip, Vector2 currentMaxClip)
{
    if (!obj || !obj->enabled) return;

    RectTransform* transform = obj->GetComponent<RectTransform>();
    if (!transform) return;

    // Collect all UI Images on this object
    for(UIImage* uiImage : obj->GetComponentsOfType<UIImage>())
    {
        if (uiImage == nullptr) continue;

        RenderCommand command;
        command.type = CommandType::RenderUI;
        command.mesh = uiImage->meshData;
        command.transform = transform->GetRectMatrix();
        command.material = uiImage->material;
        command.texture = uiImage->texture;
        command.zOrder = zOrder;
        command.minClipBounds = currentMinClip;
        command.maxClipBounds = currentMaxClip;
        command.name = obj->name;

        renderQueue.push_back(command);
    }

    // Collect all UI Text components on this object
    for(UIText* uiText : obj->GetComponentsOfType<UIText>())
    {
        if (!uiText) continue;

        if (uiText->font && !uiText->text.empty() && uiText->meshData)
        {
            RenderCommand command;
            command.type = CommandType::RenderUIText;
            command.mesh = uiText->meshData;

            glm::mat4 rectMat = transform->GetRectMatrix();

            float scaleX = glm::length(glm::vec3(rectMat[0]));
            float scaleY = glm::length(glm::vec3(rectMat[1]));

            glm::vec3 right   = glm::vec3(rectMat[0]) / (scaleX > 0.0001f ? scaleX : 1.0f);
            glm::vec3 up      = glm::vec3(rectMat[1]) / (scaleY > 0.0001f ? scaleY : 1.0f);
            glm::vec3 forward = glm::normalize(glm::vec3(rectMat[2]));

            glm::mat4 unscaledMat = glm::mat4(1.0f);
            unscaledMat[0] = glm::vec4(right, 0.0f);
            unscaledMat[1] = glm::vec4(up, 0.0f);
            unscaledMat[2] = glm::vec4(forward, 0.0f);

            Vector2 topLeft = transform->GetAbsolutePosition();
            unscaledMat[3] = glm::vec4(topLeft.x, topLeft.y, rectMat[3].z, 1.0f);

            command.transform = unscaledMat;
            command.material = uiText->material;
            command.font = uiText->font;
            command.texture = uiText->font->fontAtlasTexture;
            command.zOrder = zOrder;
            command.fontWeight = uiText->fontWeight;
            command.minClipBounds = currentMinClip;
            command.maxClipBounds = currentMaxClip;
            command.name = obj->name;

            renderQueue.push_back(command);
        }
    }

    // Determine the clipping bounds to pass to children
    Vector2 nextMinClip = currentMinClip;
    Vector2 nextMaxClip = currentMaxClip;

    if (transform->clipChildren)
    {
        Vector2 myMin = transform->GetAbsolutePosition();
        Vector2 myMax = transform->GetAbsolutePosition() + transform->GetAbsoluteSize();

        nextMinClip.x = std::max(currentMinClip.x, myMin.x);
        nextMinClip.y = std::max(currentMinClip.y, myMin.y);
        nextMaxClip.x = std::min(currentMaxClip.x, myMax.x);
        nextMaxClip.y = std::min(currentMaxClip.y, myMax.y);
    }

    for (Object* child : obj->children)
    {
        CollectUIHierarchy(child, renderQueue, zOrder + 1, nextMinClip, nextMaxClip);
    }
}

void Renderer::RenderMesh(RenderCommand command)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if(lastShader != command.material->shader->shaderId)
    {
        command.material->shader->Use();

        int lightCount = std::min((int)Engine::lights.size(), 10); 
        command.material->shader->SetInt("lightsInScene", lightCount);

        for(int i = 0; i < lightCount; i++)
        {
            Light* light = Engine::lights.at(i);
            Transform* lightTransform = light->GetComponent<Transform>();

            string baseName = "lights[" + to_string(i) + "].";

            command.material->shader->SetVector3(baseName + "color", light->color);
            command.material->shader->SetVector3(baseName + "position", lightTransform->position);
            command.material->shader->SetFloat(baseName + "strength", light->strength);

            if(light->type == LightType::Directional)
            {
                command.material->shader->SetInt(baseName + "type", light->type);
                command.material->shader->SetVector3(baseName + "direction", (lightTransform->rotation * Vector3(0.0f, 0.0f, -1.0f)));
            }
            else
            {
                command.material->shader->SetInt(baseName + "type", 0);
            }
        }
    }

    if(lastMaterial != command.material)
    {
        command.material->ApplyMaterial();
        lastTexture = nullptr;
    }

    if(lastMesh != command.mesh)
    {
        command.mesh->vao.Bind();
    }

    RenderStaticMesh(command);

    lastShader = command.material->shader->shaderId;
    lastMesh = command.mesh;
    lastMaterial = command.material;
}

void Renderer::RenderStaticMesh(RenderCommand command)
{
    command.material->shader->SetMat4("Model", command.transform);
    command.material->shader->SetMat4("MVP", viewProjectionMatrix * command.transform);

    glDrawArrays(GL_TRIANGLES, 0, command.mesh->vertices.size() / 3);
}

void Renderer::RenderUI(RenderCommand command)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); 
    glEnable(GL_BLEND);       
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 1. Force Shader Bind
    if(lastShader != command.material->shader->shaderId)
    {
        command.material->shader->Use();
        lastShader = command.material->shader->shaderId;
    }
    command.material->shader->Use(); 

    // 2. Apply Material & Invalidate Texture Cache if Material Changed
    if(lastMaterial != command.material)
    {
        command.material->ApplyMaterial();
        lastMaterial = command.material;
        lastTexture = nullptr; // Force rebind of textures since ApplyMaterial changes bound GL textures
    }

    if(lastMesh != command.mesh)
    {
        command.mesh->vao.Bind();
        lastMesh = command.mesh;
    }

    // 3. Texture Binding Strategy
    if(command.texture)
    {
        if(lastTexture != command.texture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, command.texture->textureId);
            lastTexture = command.texture;
        }
    }
    else
    {
        // If this UI command has no direct texture, invalidate cache
        lastTexture = nullptr;
    }

    if(command.fontWeight != lastWeight)
    {
        command.material->shader->SetFloat("weight", command.fontWeight);
        lastWeight = command.fontWeight;
    }

    command.material->shader->SetMat4("MVP", orthographicMatrix * command.transform);

    // 4. Calculate Clipping Bounds
    float x1 = command.minClipBounds.x;
    float x2 = command.maxClipBounds.x;

    float y1 = Engine::windowDimensions.y - command.minClipBounds.y;
    float y2 = Engine::windowDimensions.y - command.maxClipBounds.y;

    float glMinX = std::min(x1, x2);
    float glMaxX = std::max(x1, x2);
    float glMinY = std::min(y1, y2);
    float glMaxY = std::max(y1, y2);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(Engine::window, &fbWidth, &fbHeight); 
    float scaleX = (float)fbWidth / (float)Engine::windowDimensions.x;
    float scaleY = (float)fbHeight / (float)Engine::windowDimensions.y;

    glMinX *= scaleX;
    glMaxX *= scaleX;
    glMinY *= scaleY;
    glMaxY *= scaleY;

    command.material->shader->SetVector4("clipBounds", glm::vec4(glMinX, glMinY, glMaxX, glMaxY));

    // 5. Render
    if(command.type == CommandType::RenderUI) // UIImage
    {
        command.material->shader->SetInt("baseTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, command.mesh->vertices.size() / 3);
    }
    else if(command.type == CommandType::RenderUIText) // UIText
    {
        RenderUIText(command);
    }
}

void Renderer::RenderUIText(RenderCommand command)
{
    command.material->shader->SetInt("textAtlas", 0);

    if (!command.mesh->indices.empty())
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(command.mesh->indices.size()), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, command.mesh->vertices.size() / 3);
    }
}

void Renderer::SortQueue()
{
    auto it = std::partition(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& cmd) {
        return cmd.type == CommandType::RenderMesh;
    });

    std::sort(renderQueue.begin(), it, [](const RenderCommand& a, const RenderCommand& b) {
        if (a.material->shader != b.material->shader) 
        {
            return a.material->shader < b.material->shader;
        }
        if(a.material != b.material)
        {
            return a.material.get() < b.material.get();
        }
        return a.mesh.get() < b.mesh.get(); 
    });

    std::stable_sort(it, renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
        if (a.zOrder != b.zOrder)
            return a.zOrder < b.zOrder;

        return a.texture.get() < b.texture.get();
    });
}