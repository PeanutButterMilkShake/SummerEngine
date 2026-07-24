#include "Transform.h"

void Transform::Update(float delta)
{
    forward = rotation * Vector3(0, 0, 1);
    right = rotation * Vector3(1.0f, 0.0f, 0.0f);
    up = rotation * Vector3(0.0f, 1.0f, 0.0f);
}