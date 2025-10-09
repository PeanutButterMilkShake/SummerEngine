#include "Camera.h"
#include "Engine.h"

Camera::Camera()
{
    if(Engine::mainCamera == nullptr)
        Engine::mainCamera = this;
}

Camera::~Camera()
{

}