#include "VAO.h"
#include "VBO.h"

VAO::VAO()
{
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);

    CheckOpenGLError("Creating VAO");
}

VAO::~VAO()
{

}

void VAO::Bind()
{
    glBindVertexArray(id);
    CheckOpenGLError("Binding VAO");
}

void VAO::Unbind()
{
    glBindVertexArray(0);
    CheckOpenGLError("Unbinding VAO");
}

void VAO::LinkAttrib(const VBO& vbo, GLuint layout, GLint size, GLenum type, GLsizei stride, void* offset) {
    vbo.Bind();
    glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);

    CheckOpenGLError("Linking attribute to VAO");

    vbo.Unbind();
}