#include "ShaderData.h"

ShaderData::ShaderData(const std::string& vertPath, const std::string& fragPath)
{
    vertexShader = CompileShaderAndCheckForError("VERTEX", ReadFile(vertPath).c_str());
    fragmentShader = CompileShaderAndCheckForError("FRAGMENT", ReadFile(fragPath).c_str());
    shaderId = CompileProgramAndCheckForError(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderData::~ShaderData() {
    glDeleteProgram(shaderId);
}

void ShaderData::Use() {
    glUseProgram(shaderId);
    CheckOpenGLError("Using shader program");
}

void ShaderData::SetMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    CheckOpenGLError("Setting uniform of type mat4: " + name);
}

void ShaderData::SetVector2(const std::string &name, const Vector2 &value) {
    glUniform2fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void ShaderData::SetVector3(const std::string &name, const Vector3 &value) {
    glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void ShaderData::SetFloat(const std::string &name, const float &value) {
    glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void ShaderData::SetInt(const std::string &name, const int &value) {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}