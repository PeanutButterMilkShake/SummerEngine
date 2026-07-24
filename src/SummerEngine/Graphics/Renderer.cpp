#include "Renderer.h"
#include "Engine.h"
#include <algorithm>

std::vector<RenderCommand> Renderer::renderQueue;
glm::mat4 Renderer::viewProjectionMatrix;
glm::mat4 Renderer::orthographicMatrix;

void Renderer::Render()
{
    // Clear queue for next frame and prevent vector resizing
    renderQueue.clear();
    renderQueue.reserve(Engine::objects.size());

    // Collect mesh components
    for(Object* object : Engine::objects)
    {
        if(Mesh* mesh = object->GetComponent<Mesh>())
        {
            RenderCommand command;
            command.type = CommandType::RenderMesh;

            command.mesh = mesh->meshData;
            command.transform = mesh->GetModelMatrix();
            command.material = mesh->material;

            renderQueue.push_back(command);
        }
        else if(UIImage* uiImage = object->GetComponent<UIImage>())
        {
            RenderCommand command;
            command.type = CommandType::RenderUI;

            command.mesh = uiImage->meshData;
            command.transform = uiImage->GetRectMatrix();
            command.material = uiImage->material;

            renderQueue.push_back(command);
        }
    }

    // Sort models by material
    SortQueue();

    // Calculate view project matrix
    viewProjectionMatrix = Engine::mainCamera->PerspectiveMatrix() * Engine::mainCamera->ViewMatrix();

    // Calculate orthographic matrix for UI FOR NOW USING HACKY UI WINDOW SIZE REFERENCE CHANGE WHEN MAKING CANVAS OBJECT
    orthographicMatrix = glm::ortho(0.f, Engine::windowDimensions.x, Engine::windowDimensions.y, 0.f, -1.f, 1000.f);

    // Render models
    int lastShader = -1;
    shared_ptr<MeshData> lastMesh = nullptr;
    shared_ptr<Material> lastMaterial = nullptr;
    for(RenderCommand command : renderQueue)
    {
        if(command.type == CommandType::RenderMesh)
        {
            RenderMesh(command);
        }
        else if(command.type == CommandType::RenderUI)
        {
            ResourceManager::Get<MeshData>("assets/Models/UIPlane.obj")->vao.Bind();

            RenderUI(command);
        }

    }
}

void Renderer::RenderMesh(RenderCommand command)
{
    // Render models
    int lastShader = -1;
    shared_ptr<MeshData> lastMesh = nullptr;
    shared_ptr<Material> lastMaterial = nullptr;

    // If using a new shader load it and set light data
    if(lastShader != command.material->shader->shaderId)
    {
        command.material->shader->Use();

        int lightCount = std::min((int)Engine::lights.size(), 10); 
        command.material->shader->SetInt("lightsInScene", lightCount);

        // Send light data to object
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

    // If using a new material set its data
    if(lastMaterial != command.material)
    {
        command.material->ApplyMaterial();
    }

    // If using a new mesh load it
    if(lastMesh != command.mesh)
    {
        command.mesh->vao.Bind();
    }

    // Render the mesh (seperated by a function because i plan to add skeletal mesh too!!)
    RenderStaticMesh(command);

    lastShader = command.material->shader->shaderId;
    lastMesh = command.mesh;
    lastMaterial = command.material;
}

void Renderer::RenderStaticMesh(RenderCommand command)
{
    // Set model matrices
    command.material->shader->SetMat4("Model", command.transform);
    command.material->shader->SetMat4("MVP", viewProjectionMatrix * command.transform);

    //Render
    glDrawElements(GL_TRIANGLES, command.mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::RenderUI(RenderCommand command)
{
    command.material->shader->Use();
    command.material->shader->SetMat4("MVP", orthographicMatrix * command.transform);
    glDrawElements(GL_TRIANGLES, command.mesh->indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::SortQueue()
{
    std::sort(renderQueue.begin(), renderQueue.end(), [](const RenderCommand& a, const RenderCommand& b) {
        // Sort by shader
        if (a.material->shader != b.material->shader) 
        {
            return a.material->shader < b.material->shader;
        }

        // Sort by material
        if(a.material != b.material)
        {
            return a.material.get() < b.material.get();
        }

        // Sort by mesh
        return a.mesh.get() < b.mesh.get(); 
    });
}