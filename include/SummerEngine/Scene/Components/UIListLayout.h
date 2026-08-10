#pragma once

#include "RectTransform.h"
#include "Object.h"
#include "UIEnums.h"

class UIListLayout : public Component
{
private:
    std::vector<Object*> lastChildren;
    std::vector<Vector3> positions;
    void CalculatePositions();
    RectTransform* transform;
public:
    
    Vector2 paddingOffset;
    Vector2 paddingScale;

    LayoutAxis layoutAxis;
    UIAlignmentHorizontal horizontalAlignment;
    UIAlignmentVertical verticalAlignment;

    void Start() override;
    void Update(float delta) override;    
};