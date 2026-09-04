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