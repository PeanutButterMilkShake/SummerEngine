#ifndef MESH_H
#define MESH_H

#include "Object.h"
#include <iostream>
#include <vector>
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

class Mesh : public Object
{
public:
    Shader *shader;
    string meshFilePath = "";

    Mesh(string filePath);
    ~Mesh();
    void Update() override;

    void Render();
    void LoadMesh();

private:
    string lastMeshFilePath = "";

    VAO vao; 
    VBO vbo;
    EBO ebo;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif