#include "ShaderData.h"

Shader::Impl::Impl(const std::string& shaderPath)
{
    std::string source = ReadFile(shaderPath);

    std::string vertSource;
    std::string fragSource;

    size_t vertToken = source.find("#type vertex");
    size_t fragToken = source.find("#type fragment");

    //Seperate shader into vertex and fragment shader
    if (vertToken != std::string::npos && fragToken != std::string::npos)
    {
        if (vertToken < fragToken)
        {
            size_t vertStart = source.find('\n', vertToken);
            if (vertStart != std::string::npos) vertStart++;

            vertSource = source.substr(vertStart, fragToken - vertStart);

            size_t fragStart = source.find('\n', fragToken);
            if (fragStart != std::string::npos) fragStart++;

            fragSource = source.substr(fragStart);
        }
        else
        {
            size_t fragStart = source.find('\n', fragToken);
            if (fragStart != std::string::npos) fragStart++;

            fragSource = source.substr(fragStart, vertToken - fragStart);

            size_t vertStart = source.find('\n', vertToken);
            if (vertStart != std::string::npos) vertStart++;

            vertSource = source.substr(vertStart);
        }
    }

    // Compile extracted sources
    int vertexShader = CompileShaderAndCheckForError("VERTEX", vertSource.c_str());
    int fragmentShader = CompileShaderAndCheckForError("FRAGMENT", fragSource.c_str());
    shaderId = CompileProgramAndCheckForError(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    InspectShaderTextures();
}


Shader::Impl::~Impl() {
    glDeleteProgram(shaderId);
}

void Shader::Impl::Use() {
    glUseProgram(shaderId);
    CheckOpenGLError("Using shader program");
}

void Shader::Impl::SetMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    CheckOpenGLError("Setting uniform of type mat4: " + name);
}

void Shader::Impl::SetVector2(const std::string &name, const Vector2 &value) {
    glUniform2fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void Shader::Impl::SetVector3(const std::string &name, const Vector3 &value) {
    glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void Shader::Impl::SetVector4(const std::string &name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void Shader::Impl::SetFloat(const std::string &name, const float &value) {
    glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::Impl::SetInt(const std::string &name, const int &value) {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::Impl::InspectShaderTextures() 
{
    GLint numUniforms = 0;
    glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &numUniforms);

    for (GLint i = 0; i < numUniforms; i++) 
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        
        glGetActiveUniform(shaderId, i, sizeof(name), &length, &size, &type, name);

        if (type == GL_SAMPLER_2D) 
        {
            std::string uniformName(name);
            requiredTextures.push_back(uniformName);
        }
    }
}