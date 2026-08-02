#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 UV;

out vec4 FragColor;

struct Light {
    vec3 color;
    vec3 position;
    float strength;
    vec3 direction;
    int type; // 0 = Directional, 1 = Point
};

uniform Light lights[32];
uniform int lightsInScene;
uniform vec3 baseColor;
uniform sampler2D baseTexture;

vec3 CalculateDirectionalLight(int i)
{
    Light light = lights[i];
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(norm, lightDir), 0.0);
    
    return texture(baseTexture, UV).rgb * baseColor * light.color * (diff * light.strength);
}

void main()
{
    // Ambient lighting base
    vec3 ambientStrength = vec3(0.1);
    vec3 result = ambientStrength * baseColor * texture(baseTexture, UV).rgb;

    // Accumulate light sources
    for(int i = 0; i < lightsInScene; i++)
    {
        if(lights[i].type == 0) // Directional Light
        {
            result += CalculateDirectionalLight(i);
        } 
    }

    FragColor = vec4(texture(baseTexture, UV).rgb, 1.0);
}