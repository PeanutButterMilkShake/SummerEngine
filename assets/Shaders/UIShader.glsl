#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 normal;
out vec2 UV;

uniform mat4 MVP;

void main()
{
    UV = aUV;
    normal = aNormal;
    gl_Position = MVP * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform vec3 baseColor;
uniform sampler2D baseTexture;
uniform vec4 clipBounds;

void main()
{
    if (gl_FragCoord.x < clipBounds.x || gl_FragCoord.x > clipBounds.z || gl_FragCoord.y < clipBounds.y || gl_FragCoord.y > clipBounds.w)
    {
        discard;
    }

    vec4 t = texture(baseTexture, UV);
    FragColor = vec4(baseColor * t.rgb, t.a);
}