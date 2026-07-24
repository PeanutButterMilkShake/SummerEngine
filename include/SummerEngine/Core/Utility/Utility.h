#ifndef UTILITY_H
#define UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "FileReader.h"

int CompileShaderAndCheckForError(const char *shaderType, const char *source);
int CompileProgramAndCheckForError(int vertexShader, int fragmentShader);
void CheckOpenGLError(const std::string& context = "");

constexpr uint32_t HashString(const char* str)
{
    uint32_t hash = 0x811C9DC5; // FNV offset basis
    while (*str)
    {
        hash ^= static_cast<uint32_t>(*str++);
        hash *= 0x01000193; // FNV prime
    }
    return hash;
}

#endif