#pragma once
#include "RectTransform.h"
#include "Object.h"
#include "UIEnums.h"
#include "UIListLayout.h.generated.h"
class UIListLayout : public Component
{
private:
    std::vector<Object*> lastChildren;
    std::vector<Vector3> positions;
    void CalculatePositions();
    RectTransform* transform;
public:
    
    SPROPERTY();
    Vector2 paddingOffset;
    SPROPERTY();
    Vector2 paddingScale;

    UIAxis layoutAxis;
    UIAlignmentHorizontal horizontalAlignment;
    UIAlignmentVertical verticalAlignment;

    void Start() override;
    void Update(float delta) override;    

    GENERATED_BODY();
};