#include "FlyCamera.h"

FlyCamera::FlyCamera()
{

}

FlyCamera::~FlyCamera()
{

}

void FlyCamera::Update()
{
    glm::vec2 lookDirection = Input::GetInputVector2(KeyCode::LeftArrow, KeyCode::RightArrow, KeyCode::DownArrow, KeyCode::UpArrow) * 0.005f;
    glm::vec2 moveDirection = Input::GetInputVector2(KeyCode::A, KeyCode::D, KeyCode::S, KeyCode::W);
    float verticalDirection = (float)Input::GetInputAxis(KeyCode::LeftControl, KeyCode::Space);

    rotation.y += lookDirection.x;
    rotation.x += lookDirection.y;

    position += (right * moveDirection.x + forward * moveDirection.y + glm::vec3(0,verticalDirection,0))  * 0.001f;
}