#version 330 core

in vec2 UV;

out vec4 FragColor;

uniform vec3 baseColor;
uniform sampler2D baseTexture;

void main()
{
    vec4 t = texture(baseTexture, UV);
    FragColor = vec4(baseColor * t.rgb, t.a);
}