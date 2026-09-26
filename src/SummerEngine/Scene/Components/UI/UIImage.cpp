#include "UIImage.h"
#include <format>

void UIImage::Start()
{
    transform = GetComponent<RectTransform>();
    meshData = MeshData("assets/Models/UIPlane.obj");
}