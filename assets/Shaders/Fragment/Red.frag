#version 330 core

in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

// Define the structure of your light
struct Light {
    vec3 color;
    vec3 position;
    float strength;
    vec3 direction; // For directional lights
    int type;       // 0 = Point Light, 1 = Directional Light
};

#define MAX_LIGHTS 32

uniform Light lights[MAX_LIGHTS];
uniform int lightsInScene;

vec3 CalculateDirectionalLight(int i)
{
    Light light = lights[i];
    return light.strength * light.color * max(dot(normalize(normal), -light.direction), 0.);
}

/*vec3 CalculatePointLight()
{
    
}*/

void main()
{
    vec3 result = vec3(0);

    vec3 ambientStrength = vec3(0.1);
    result += ambientStrength * vec3(1,0,0);

    for(int i = 0; i < lightsInScene; i++)
    {
        if(lights[i].type == 1) // Directional Light
        {
            result += CalculateDirectionalLight(i);
        } 
    }

    FragColor = vec4(result, 1.0);
}