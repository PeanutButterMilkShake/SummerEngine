#pragma once

#include <string>
#include "MathTypes.h"
#include "Utility.h"
#include "Resource.h"

class Shader : Resource
{
public:
    unsigned int shaderId;
    std::vector<std::string> requiredTextures;

    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    void Use();

    void SetInt(const std::string &name, const int &value);
    void SetFloat(const std::string &name, const float &value);
    void SetVector2(const std::string &name, const Vector2 &value);
    void SetVector3(const std::string &name, const Vector3 &value);
    void SetMat4(const std::string &name, const glm::mat4 &mat);

    void InspectShaderTextures();
};