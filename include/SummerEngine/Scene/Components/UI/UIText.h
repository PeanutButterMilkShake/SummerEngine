#pragma once
#include "RectTransform.h"
#include "MaterialData.h"
#include "Component.h"
#include "MeshData.h"
#include "ResourceManager.h"
#include "Object.h"
#include "Font.h"
#include <string>
#include "UIEnums.h"
#include "UIText.h.generated.h"
class UIText : public Component
{
private:
    std::string lastText = "";
    float lastFontSize = -1.0f;
    std::string lastFontPath = "";
    
    float lastCharPadding = 0.0f;
    float lastWordPadding = 0.0f;
    float lastLinePadding = 0.0f;
    UIAlignmentHorizontal lastHAlign = UIAlignmentHorizontal::Left;
    UIAlignmentVertical lastVAlign = UIAlignmentVertical::Center;

    Vector2 lastBoxSize = Vector2(-1.0f, -1.0f);

public:
    RectTransform* transform = nullptr;
    std::shared_ptr<MeshData> meshData = nullptr;
    std::shared_ptr<Material> material = nullptr;
    std::shared_ptr<Font> font = nullptr;

    // Public properties
    std::string fontFilePath = "assets/Fonts/JetBrainsMono-Regular.ttf";
    SPROPERTY();
    float fontSize = 32.0f;
    SPROPERTY();
    float fontWeight = 0.0f;
    std::string text = "Default Text";
    
    // Formatting & Layout Properties
    SPROPERTY();
    float characterPadding = 0.0f;
    SPROPERTY();
    float wordPadding = 0.0f;
    SPROPERTY();
    float linePadding = 0.0f;
    UIAlignmentHorizontal horizontalAlignment = UIAlignmentHorizontal::Left;
    UIAlignmentVertical verticalAlignment = UIAlignmentVertical::Center;

    UIText();
    void Start() override;
    void Update(float delta) override;

    void GenerateTextMesh();

    GENERATED_BODY();
};