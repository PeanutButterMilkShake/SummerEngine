#include "Light.h"

Light::Light()
{
    Engine::lights.push_back(this);
} 