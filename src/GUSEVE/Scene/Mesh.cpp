#include "Mesh.h"
#include "Engine.h"

#include <glm/gtx/string_cast.hpp>

Mesh::Mesh(string filePath) : meshFilePath(filePath)
{
    if(meshFilePath == "")
        return;

    LoadMesh();
}

Mesh::~Mesh()
{

}

void Mesh::Update()
{
    if(meshFilePath == "")
        return;
    if(meshFilePath != lastMeshFilePath)
        LoadMesh();

    Render();
}

void Mesh::Render()
{
    glm::mat4 mvp = glm::mat4(1.0f);

    mvp = glm::translate(mvp, worldPosition);

    mvp = glm::rotate(mvp, glm::radians(worldRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mvp = glm::rotate(mvp, glm::radians(worldRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = glm::rotate(mvp, glm::radians(worldRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    mvp = glm::scale(mvp, worldScale);

    mvp = Engine::mainCamera->PerspectiveMatrix() * Engine::mainCamera->ViewMatrix() * mvp;

    shader->Use();
    shader->SetMat4("MVP", mvp);

    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::LoadMesh()
{
    ReadMeshFile(meshFilePath, vertices, indices);

    vbo.SetData(vertices.data(), vertices.size() * sizeof(float));
    ebo.SetData(indices.data(), indices.size() * sizeof(unsigned int));

    vao.Bind();
    ebo.Bind();
    vbo.Bind();

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);

    vbo.Unbind();
    vao.Unbind();

    lastMeshFilePath = meshFilePath;
}