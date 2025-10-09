#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Utility.h>

class Shader
{
public:
    unsigned int shaderProgram;

    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~Shader();

    void Use();
    void SetMat4(const std::string &name, const glm::mat4 &mat);

private:
    unsigned int vertexShader;
    unsigned int fragmentShader;
};

#endif