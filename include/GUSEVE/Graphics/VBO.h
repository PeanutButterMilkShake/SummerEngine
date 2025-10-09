#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Utility.h"

class VBO
{
public:
    unsigned int id;

    VBO();
    ~VBO();

    void SetData(const void *data, GLsizeiptr size);
    void Bind() const;
    void Unbind() const;

private:

};

#endif