#include "ShaderData.h"

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
{
    int vertexShader = CompileShaderAndCheckForError("VERTEX", ReadFile(vertPath).c_str());
    int fragmentShader = CompileShaderAndCheckForError("FRAGMENT", ReadFile(fragPath).c_str());
    shaderId = CompileProgramAndCheckForError(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    InspectShaderTextures();
}

Shader::~Shader() {
    glDeleteProgram(shaderId);
}

void Shader::Use() {
    glUseProgram(shaderId);
    CheckOpenGLError("Using shader program");
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    CheckOpenGLError("Setting uniform of type mat4: " + name);
}

void Shader::SetVector2(const std::string &name, const Vector2 &value) {
    glUniform2fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void Shader::SetVector3(const std::string &name, const Vector3 &value) {
    glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, &value.x);
}

void Shader::SetFloat(const std::string &name, const float &value) {
    glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::SetInt(const std::string &name, const int &value) {
    glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
}

void Shader::InspectShaderTextures() 
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