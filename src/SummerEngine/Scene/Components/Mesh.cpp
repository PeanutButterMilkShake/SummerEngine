#include "Mesh.h"
#include "Engine.h"
#include "ResourceManager.h"

#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(string filePath) : modelFilePath(filePath) {}

void Mesh::Start() 
{
    transform = GetComponent<Transform>();

    if(modelFilePath == "")
    return;

    LoadMesh();
}

void Mesh::Update(float delta)
{
    if(modelFilePath == "")
        return;
    if(modelFilePath != lastModelFilePath)
        LoadMesh();
}

glm::mat4 Mesh::GetModelMatrix()
{
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), (glm::vec3)transform->position);
    
    // Convert Quaternion directly to a 4x4 rotation matrix
    glm::mat4 rotationMat = glm::mat4_cast((glm::quat)transform->rotation);
    
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), (glm::vec3)transform->scale);

    return translation * rotationMat * scaleMat;
}

void Mesh::LoadMesh()
{
    meshData = ResourceManager::Get<MeshData>(modelFilePath);
}