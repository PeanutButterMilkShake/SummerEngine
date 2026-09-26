#pragma once

#include <string>
#include "ResourceManager.h"

template<typename T>
class Asset
{
    std::string relativePath
    std::shared_ptr<T> resource;

    void SetPath(std::string path)
    {
        relativePath = path;
        if (!relativePath.empty())
        {
            resource = ResourceManager::GetResource<T>(path)
        }
        else
        {
            resource = nullptr;
        }
    }
};