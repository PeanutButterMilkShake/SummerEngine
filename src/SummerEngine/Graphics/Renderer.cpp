#include "Renderer.h"
#include "Engine.h"
#include <algorithm>

std::vector<RenderCommand> Renderer::renderQueue;
glm::mat4 Renderer::viewProjectionMatrix;
glm::mat4 Renderer::orthographicMatrix;
int Renderer::lastShader = -1;
shared_ptr<MeshData> Renderer::lastMesh = nullptr;
shared_ptr<Material> Renderer::lastMaterial = nullptr;
shared_ptr<Texture> Renderer::lastTexture = nullptr;

void Renderer::Render()
{
    renderQueue.clear();

    // Collect 3D mesh components
    for(Object* object : Engine::objects)
    {
        if(!object->enabled)
            continue;
        if(Mesh* mesh = object->GetComponent<Mesh>())
        {
            RenderCommand command;
            command.type = CommandType::RenderMesh;
            command.mesh = mesh->meshData;
            command.transform = mesh->GetModelMatrix();
            command.material = mesh->material;

            renderQueue.push_back(command);
        }
    }

    // Collect UI hierarchy (change to add an invisible z offset to all children)
    for(Object* object : Engine::objects)
    {
        if(object->enabled && object->parent == nullptr && object->GetComponent<RectTransform>())
        {
            CollectUIHierarchy(object, renderQueue, 0);
        }
    }

    // Sort queue by shader, material, mesh
    SortQueue();

    // Calculate view project matrix
    viewProjectionMatrix = Engine::mainCamera->PerspectiveMatrix() * Engine::mainCamera->ViewMatrix();

    // Calculate orthographic matrix for UI
    orthographicMatrix = glm::ortho(0.f, Engine::windowDimensions.x, Engine::windowDimensions.y, 0.f, -1.f, 1000.f);

    for(RenderCommand command : renderQueue)
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

void Renderer::CollectUIHierarchy(Object* obj, std::vector<RenderCommand>& renderQueue, int zOrder)
{
    if (!obj || !obj->enabled) return;

    if(UIImage* uiImage = obj->GetComponent<UIImage>())
    {
        RenderCommand command;
        command.type = CommandType::RenderUI;
        command.mesh = uiImage->meshData;
        command.transform = uiImage->GetComponent<RectTransform>()->GetRectMatrix();
        command.material = uiImage->material;
        command.texture = uiImage->texture;
        command.zOrder = zOrder;

        renderQueue.push_back(command);
    }
    else if(UIText* uiText = obj->GetComponent<UIText>())
    {
        if (uiText->font && !uiText->text.empty() && uiText->meshData)
        {
            RenderCommand command;
            command.type = CommandType::RenderUIText;
            command.mesh = uiText->meshData;

            RectTransform* rectTransform = uiText->GetComponent<RectTransform>();
            glm::mat4 rectMat = rectTransform->GetRectMatrix();
            
            float scaleX = glm::length(glm::vec3(rectMat[0]));
            float scaleY = glm::length(glm::vec3(rectMat[1]));

            glm::vec3 right   = glm::vec3(rectMat[0]) / (scaleX > 0.0001f ? scaleX : 1.0f);
            glm::vec3 up      = glm::vec3(rectMat[1]) / (scaleY > 0.0001f ? scaleY : 1.0f);
            glm::vec3 forward = glm::normalize(glm::vec3(rectMat[2]));

            glm::mat4 unscaledMat = glm::mat4(1.0f);
            unscaledMat[0] = glm::vec4(right, 0.0f);
            unscaledMat[1] = glm::vec4(up, 0.0f);
            unscaledMat[2] = glm::vec4(forward, 0.0f);
            unscaledMat[3] = rectMat[3];
            
            command.transform = unscaledMat;
            command.material = uiText->material;
            command.font = uiText->font;
            command.texture = uiText->font->fontAtlasTexture;
            command.zOrder = zOrder;
    
            renderQueue.push_back(command);
        }
    }

    for (Object* child : obj->children)
    {
        CollectUIHierarchy(child, renderQueue, zOrder + 1);
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

    if(lastShader != command.material->shader->shaderId)
        command.material->shader->Use();

    if(lastMaterial != command.material)
        command.material->ApplyMaterial();

    if(lastMesh != command.mesh)
        command.mesh->vao.Bind();

    // Bind whatever texture this command actually needs (UIImage's own texture,
    // or the font atlas for text) rather than pulling it off a shared material.
    // Cached against lastTexture, so consecutive commands with the same texture
    // (which SortQueue now groups together) skip the rebind entirely.
    if(command.texture && lastTexture != command.texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, command.texture->textureId);
        lastTexture = command.texture;
    }

    command.material->shader->SetMat4("MVP", orthographicMatrix * command.transform);

    if(command.type == CommandType::RenderUI) // UIImage
    {
        command.material->shader->SetInt("baseTexture", 0);
        glDrawArrays(GL_TRIANGLES, 0, command.mesh->vertices.size() / 3);
    }
    else if(command.type == CommandType::RenderUIText) // UIText
    {
        RenderUIText(command);
    }

    lastShader = command.material->shader->shaderId;
    lastMesh = command.mesh;
    lastMaterial = command.material;
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

    glEnable(GL_CULL_FACE); 
}

void Renderer::SortQueue()
{
    // Partition so 3D meshes are at the front, UI commands at the back
    auto it = std::partition(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& cmd) {
        return cmd.type == CommandType::RenderMesh;
    });

    // Sort 3D meshes by shader, material, mesh (unchanged)
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

    // Sort UI commands: zOrder (root panel index) is the primary key, so panels
    // never draw out of their intended stacking order relative to each other.
    // Texture is only used to break ties within the same zOrder, batching
    // draw calls that share a bound texture. stable_sort preserves the original
    // hierarchy traversal order for equal (zOrder, texture) pairs.
    std::stable_sort(it, renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
        if (a.zOrder != b.zOrder)
            return a.zOrder < b.zOrder;

        return a.texture.get() < b.texture.get();
    });
}