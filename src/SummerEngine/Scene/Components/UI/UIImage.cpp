#include "UIImage.h"
#include <format>

void UIImage::Start()
{
    transform = GetComponent<RectTransform>();
    meshData = ResourceManager::CreateResource<MeshData>("assets/Models/UIPlane.obj");
}