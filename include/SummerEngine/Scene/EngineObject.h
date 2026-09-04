#pragma once

#include "Object.h"
#include <vector>

class EngineObject : public Object
{
public:
    EngineObject();
    ~EngineObject();

    std::vector<EngineObject*> children;

    EngineObject* GetChildWithName(std::string childName);
    std::vector<EngineObject*> GetDescendants();
};