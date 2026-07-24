#include "Utility.h"

int CompileShaderAndCheckForError(const char* shaderType, const char* source)
{
    int shader;
    int success;
    char infoLog[512];

    if(std::string(shaderType) == "FRAGMENT")
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        }
    else
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
    }

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR SHADER " << shaderType << " COMPILATION FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

int CompileProgramAndCheckForError(int vertexShader, int fragmentShader)
{
    int program;
    int success;
    char infoLog[512];

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR SHADER PROGRAM COMPILATION FAILED\n" << infoLog << std::endl;
    }

    return program;
}

void CheckOpenGLError(const std::string& context) {
    GLenum error;
    bool hasError = false;

    while ((error = glGetError()) != GL_NO_ERROR) {
        hasError = true;
        std::string errorString;

        switch (error) {
            case GL_INVALID_ENUM:                  errorString = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 errorString = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             errorString = "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                errorString = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               errorString = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 errorString = "GL_OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            default:                               errorString = "UNKNOWN_ERROR"; break;
        }

        std::cerr << "[OpenGL Error] " << errorString;
        if (!context.empty()) {
            std::cerr << " | Context: " << context;
        }
        std::cerr << std::endl;
    }
}