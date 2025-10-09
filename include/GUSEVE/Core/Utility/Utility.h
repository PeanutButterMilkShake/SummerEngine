#ifndef UTILITY_H
#define UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "FileReader.h"

int CompileShaderAndCheckForError(const char *shaderType, const char *source);
int CompileProgramAndCheckForError(int vertexShader, int fragmentShader);
void CheckOpenGLError(const std::string& context = "");

#endif