#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Utility.h"

class EBO
{
public:
    unsigned int id;

    EBO();
    ~EBO();

    void SetData(const GLuint *indices, GLsizeiptr size);
    void Bind();
    void Unbind();

private:
};

#endif