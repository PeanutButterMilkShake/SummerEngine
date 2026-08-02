#include "EnginePanel.h"

EnginePanel::EnginePanel(std::string _title, std::string _category, Vector2 _defaultLocation) : title(_title), category(_category), defaultLocation(_defaultLocation)
{
    std::shared_ptr<Material> whiteMaterial  = ResourceManager::CreateResource<Material>("WhiteMaterial", "assets/Shaders/Vertex/shader.vert", "assets/Shaders/Fragment/UIshader.frag");
    whiteMaterial->SetProperty("baseColor", Color3(1,1,1));

    panelObject.AddComponent<RectTransform>()->size = {200,400};
    panelObject.GetComponent<RectTransform>()->anchor = defaultLocation;
    panelObject.GetComponent<RectTransform>()->pivot = defaultLocation;
    panelBackground = panelObject.AddComponent<UIImage>();
    panelBackground->material = whiteMaterial;
}   