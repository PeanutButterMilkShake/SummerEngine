#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Utility.h"

class VBO;

class VAO
{
public:
    unsigned int id;

    VAO();
    ~VAO();

    void Bind();
    void Unbind();
    void LinkAttrib(const VBO& vbo, GLuint layout, GLint size, GLenum type, GLsizei stride, void* offset);

private:
};

#endif