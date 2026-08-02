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

        bool hasNormals = !normals.empty();
        bool hasUVs = !uvs.empty();

        for (size_t i = 0; i < vertexCount; ++i)
        {
            // Position
            interleaved.push_back(vertices[i * 3]);
            interleaved.push_back(vertices[i * 3 + 1]);
            interleaved.push_back(vertices[i * 3 + 2]);

            // Normal
            if (hasNormals)
            {
                interleaved.push_back(normals[i * 3]);
                interleaved.push_back(normals[i * 3 + 1]);
                interleaved.push_back(normals[i * 3 + 2]);
            }

            // UV
            if (hasUVs)
            {
                interleaved.push_back(uvs[i * 2]);
                interleaved.push_back(uvs[i * 2 + 1]);
            }
        }

        unsigned int strideFloats = 3;
        if (hasNormals) strideFloats += 3;
        if (hasUVs) strideFloats += 2;
        GLsizei strideBytes = strideFloats * sizeof(float);

        vao.Bind();
        vbo.SetData(interleaved.data(), interleaved.size() * sizeof(float));
        // (Optional: You can skip ebo.SetData entirely since you aren't indexing)

        size_t offset = 0;
        vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, strideBytes, (void*)offset);
        offset += 3 * sizeof(float);

        if (hasNormals)
        {
            vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, strideBytes, (void*)offset);
            offset += 3 * sizeof(float);
        }

        if (hasUVs)
        {
            vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, strideBytes, (void*)offset);
            offset += 2 * sizeof(float);
        }

        vao.Unbind();
        vbo.Unbind();
    }
};