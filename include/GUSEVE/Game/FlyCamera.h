#ifndef FLYCAMERA_H
#define FLYCAMERA_H

#include "Object.h"
#include "Engine.h"

class FlyCamera : public Object
{
public:
    FlyCamera();
    ~FlyCamera();

    void Update() override;
private:

};

#endif