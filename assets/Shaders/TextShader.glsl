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

uniform sampler2D textAtlas;
uniform vec3 baseColor;
uniform vec4 clipBounds;
uniform float weight;

void main()
{
    if (gl_FragCoord.x < clipBounds.x || gl_FragCoord.x > clipBounds.z || gl_FragCoord.y < clipBounds.y || gl_FragCoord.y > clipBounds.w)
    {
        discard;
    }

    vec2 texelSize = vec2(1.0 / 1024.0, 1.0 / 1024.0);

    const float onEdge = 180.0 / 255.0;
    
    // Adjust the edge threshold based on weight.
    // A positive weight value lowers the threshold, expanding the glyph pixels (making it bold).
    float threshold = onEdge - weight;

    float d0 = texture(textAtlas, UV - vec2(texelSize.x * 0.5, 0.0)).r;
    float d1 = texture(textAtlas, UV).r;
    float d2 = texture(textAtlas, UV + vec2(texelSize.x * 0.5, 0.0)).r;

    float w = fwidth(d1) * 0.7;

    float a0 = smoothstep(threshold - w, threshold + w, d0);
    float a1 = smoothstep(threshold - w, threshold + w, d1);
    float a2 = smoothstep(threshold - w, threshold + w, d2);

    float alpha = (a0 + a1 + a2) / 3.0;
    if (alpha < 0.01) discard;

    FragColor = vec4(baseColor, alpha);
}