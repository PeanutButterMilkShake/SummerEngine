#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_Transform_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_Transform_h_23() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"position", PropertyType::Vector3, [this](){ return std::to_string(position.x) + "," + std::to_string(position.y) + "," + std::to_string(position.z); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f,%f", &position.x, &position.y, &position.z); }}); \
        properties.push_back({"scale", PropertyType::Vector3, [this](){ return std::to_string(scale.x) + "," + std::to_string(scale.y) + "," + std::to_string(scale.z); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f,%f", &scale.x, &scale.y, &scale.z); }}); \
        return properties; \
    }
