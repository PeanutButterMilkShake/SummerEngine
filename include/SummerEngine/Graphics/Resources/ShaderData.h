#pragma once

#include <string>
#include "MathTypes.h"
#include "Utility.h"
#include "Resource.h"

class ShaderData : Resource
{
    public:
    unsigned int shaderId;

    ShaderData(const std::string& vertPath, const std::string& fragPath);
    ~ShaderData();

    void Use();

    void SetInt(const std::string &name, const int &value);
    void SetFloat(const std::string &name, const float &value);
    void SetVector2(const std::string &name, const Vector2 &value);
    void SetVector3(const std::string &name, const Vector3 &value);
    void SetMat4(const std::string &name, const glm::mat4 &mat);

    private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
};