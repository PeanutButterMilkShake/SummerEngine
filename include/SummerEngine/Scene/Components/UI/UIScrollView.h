#pragma once
#include "Component.h"
#include "RectTransform.h"
#include "UIEnums.h"
#include  "Input.h"
#include  "Math.h"
#include "UIScrollView.h.generated.h"
class UIScrollView : public Component
{
public:
    UIAxis scrollDirection = UIAxis::Vertical;
    SPROPERTY();
    Vector2 scrollViewSize = {0,2};
    SPROPERTY();
    float scrollSpeed = 1;
    bool invertedScrollView = false;

    void Start() override;
    void Update(float delta)override;
private:
    Vector2 currentScrollOffset;
    RectTransform* transform;

    GENERATED_BODY();
};