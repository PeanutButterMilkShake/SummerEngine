#include "Shader.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    vertexShader = CompileShaderAndCheckForError("VERTEX", ReadFile(vertexShaderSource).c_str());
    fragmentShader = CompileShaderAndCheckForError("FRAGMENT", ReadFile(fragmentShaderSource).c_str());
    shaderProgram = CompileProgramAndCheckForError(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void Shader::Use() {
    glUseProgram(shaderProgram);
    CheckOpenGLError("Using shader program");
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    CheckOpenGLError("Setting uniform of type mat4: " + name);
}
