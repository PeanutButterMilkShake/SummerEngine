#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 MVP;
uniform mat4 Model;

out vec3 fragPos;
out vec3 normal;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
    fragPos = aPos;

    normal = mat3(transpose(inverse(Model))) * aNormal;
}