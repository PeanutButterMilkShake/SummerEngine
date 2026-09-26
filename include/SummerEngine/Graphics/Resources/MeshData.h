#pragma once

#include <vector>
#include <string>
#include "glad/glad.h"
#include "FileReader.h"
#include "EBO.h"
#include "VAO.h"
#include "VBO.h"
#include "Resource.h"
#include "ResourceManager.h"
#include <memory>

struct MeshData
{
public:
    struct Impl : public Resource
    {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<unsigned int> indices;

        VAO vao;
        VBO vbo;
        EBO ebo;

        Impl() = default;

        Impl(const std::string& filePath)
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
                interleaved.push_back(vertices[i * 3]);
                interleaved.push_back(vertices[i * 3 + 1]);
                interleaved.push_back(vertices[i * 3 + 2]);

                if (hasNormals)
                {
                    interleaved.push_back(normals[i * 3]);
                    interleaved.push_back(normals[i * 3 + 1]);
                    interleaved.push_back(normals[i * 3 + 2]);
                }

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

            if (!indices.empty())
            {
                ebo.SetData(indices.data(), indices.size() * sizeof(unsigned int));
            }

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
            if (!indices.empty())
            {
                ebo.Unbind();
            }
        }
    };

    std::shared_ptr<Impl> m_impl;

    MeshData(std::nullptr_t) : m_impl(nullptr) {}

    MeshData() : m_impl(std::make_shared<Impl>()) {}

    MeshData(const std::string& filePath)
    {
        m_impl = ResourceManager::GetResource<Impl>(filePath);
        if (m_impl) return;

        m_impl = std::make_shared<Impl>(filePath);
        ResourceManager::AddToCache<Impl>(filePath, m_impl);
    }

    // equality check
    bool operator==(const MeshData& other) const { return m_impl == other.m_impl; }
    bool operator!=(const MeshData& other) const { return m_impl != other.m_impl; }

    bool operator==(std::nullptr_t) const { return m_impl == nullptr; }
    bool operator!=(std::nullptr_t) const { return m_impl != nullptr; }

    bool operator<(const MeshData& other) const { return m_impl.get() < other.m_impl.get(); }
    bool operator>(const MeshData& other) const { return m_impl.get() > other.m_impl.get(); }

    Impl* operator->() { return m_impl.get(); }
    const Impl* operator->() const { return m_impl.get(); }
    explicit operator bool() const { return m_impl != nullptr; }
};