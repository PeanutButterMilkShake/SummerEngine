#include "UIImage.h"

void UIImage::Start()
{
    transform = GetComponent<RectTransform>();
    meshData = ResourceManager::Get<MeshData>("assets/Models/UIPlane.obj");
    material = ResourceManager::Get<Material>("UI", "assets/Shaders/Vertex/UIShader.vert", "assets/Shaders/Fragment/UIShader.frag");
}

glm::mat4 UIImage::GetRectMatrix()
{
    // Center image
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(Engine::windowDimensions.x * 0.5f, Engine::windowDimensions.y * 0.5f, 0));

    // Offset image by position and anchor
    model = glm::translate(model, glm::vec3(transform->position.x + transform->anchor.x * (Engine::windowDimensions.x * 0.5f), transform->position.y + transform->anchor.y * (Engine::windowDimensions.y * 0.5f), 0/*transform->position.z*/));

    // Rotate
    //model = model * glm::mat4_cast((glm::quat)transform->rotation);

    // Scale image depending on current window dimensions
    Vector2 scaleFactor = (referenceResolution / Engine::windowDimensions);
    model = glm::scale(model, glm::vec3(transform->size.x / scaleFactor.y, transform->size.y / scaleFactor.y, 1.0f));

    //Offset image by pivot
    model = glm::translate(model, glm::vec3(transform->pivot.x * .5f, transform->pivot.y * 0.5f, 0));

    return model;
}