#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_Camera_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_Camera_h_27() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"fov", PropertyType::Float, [this](){ return std::to_string(fov); }, [this](const std::string& value){ fov = std::stof(value); }}); \
        properties.push_back({"nearPlane", PropertyType::Float, [this](){ return std::to_string(nearPlane); }, [this](const std::string& value){ nearPlane = std::stof(value); }}); \
        properties.push_back({"farPlane", PropertyType::Float, [this](){ return std::to_string(farPlane); }, [this](const std::string& value){ farPlane = std::stof(value); }}); \
        return properties; \
    }
