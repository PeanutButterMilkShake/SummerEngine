#include "EBO.h"

EBO::EBO()
{
    glGenBuffers(1, &id);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}

void EBO::SetData(const GLuint* indices, GLsizeiptr size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    CheckOpenGLError("Setting EBO data");
}

void EBO::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    CheckOpenGLError("Binding EBO");
}

void EBO::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CheckOpenGLError("Unbinding EBO");
}