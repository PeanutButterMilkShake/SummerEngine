#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D textAtlas;
uniform vec3 baseColor;

void main()
{
    vec2 texelSize = {1. / 1024, 1./ 1024};

    const float onEdge = 180.0 / 255.0;

    float d0 = texture(textAtlas, UV - vec2(texelSize.x * 0.5, 0.0)).r;
    float d1 = texture(textAtlas, UV).r;
    float d2 = texture(textAtlas, UV + vec2(texelSize.x * 0.5, 0.0)).r;

    float w = fwidth(d1) * 0.7;

    float a0 = smoothstep(onEdge - w, onEdge + w, d0);
    float a1 = smoothstep(onEdge - w, onEdge + w, d1);
    float a2 = smoothstep(onEdge - w, onEdge + w, d2);

    float alpha = (a0 + a1 + a2) / 3.0;
    if (alpha < 0.01) discard;

    FragColor = vec4(baseColor, alpha);
}