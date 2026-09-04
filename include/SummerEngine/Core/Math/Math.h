#pragma once

#include <algorithm>
#include "MathTypes.h"

inline float Clamp(float x, float min, float max)
{
    return std::clamp(x, min, max);
}

inline Vector2 Clamp(Vector2 x, Vector2 min, Vector2 max)
{
    return Vector2(Clamp(x.x, min.x, max.x), Clamp(x.y, min.y, max.y));
}