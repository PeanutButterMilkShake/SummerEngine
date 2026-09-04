#pragma once


class Object;
class EngineObject;
class UIImage;

class PanelContent
{
public:
    static PanelContent* instance;

    virtual void Init();
    virtual void Update();
    virtual ~PanelContent() = default;
    
private:
};