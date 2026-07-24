#pragma once

#include <vector>
#include <string>
#include "glad/glad.h"
#include "FileReader.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Resource.h"

struct MeshData : public Resource
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    VAO vao;
    VBO vbo;
    EBO ebo;

    MeshData(std::string filePath)
    {
        ReadMeshFile(filePath, vertices, indices, normals, uvs);
        LoadMesh();
    }

    void LoadMesh()
    {
        std::vector<float> interleaved;
        size_t vertexCount = vertices.size() / 3;

        for (size_t i = 0; i < vertexCount; ++i)
        {
            // Position
            interleaved.push_back(vertices[i*3]);
            interleaved.push_back(vertices[i*3 + 1]);
            interleaved.push_back(vertices[i*3 + 2]);

            // Normal
            if (!normals.empty())
            {
                interleaved.push_back(normals[i*3]);
                interleaved.push_back(normals[i*3 + 1]);
                interleaved.push_back(normals[i*3 + 2]);
            }
        }

        vbo.SetData(interleaved.data(), interleaved.size() * sizeof(float));
        ebo.SetData(indices.data(), indices.size() * sizeof(unsigned int));

        vao.Bind();
        ebo.Bind();
        vbo.Bind();

        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0); // Position
        vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Normal

        vbo.Unbind();
        vao.Unbind();
    }

};