#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_UI_RectTransform_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_UI_RectTransform_h_61() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"sizeScale", PropertyType::Vector2, [this](){ return std::to_string(sizeScale.x) + "," + std::to_string(sizeScale.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &sizeScale.x, &sizeScale.y); }}); \
        properties.push_back({"sizeOffset", PropertyType::Vector2, [this](){ return std::to_string(sizeOffset.x) + "," + std::to_string(sizeOffset.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &sizeOffset.x, &sizeOffset.y); }}); \
        properties.push_back({"positionScale", PropertyType::Vector2, [this](){ return std::to_string(positionScale.x) + "," + std::to_string(positionScale.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &positionScale.x, &positionScale.y); }}); \
        properties.push_back({"positionOffset", PropertyType::Vector2, [this](){ return std::to_string(positionOffset.x) + "," + std::to_string(positionOffset.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &positionOffset.x, &positionOffset.y); }}); \
        properties.push_back({"pivot", PropertyType::Vector2, [this](){ return std::to_string(pivot.x) + "," + std::to_string(pivot.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &pivot.x, &pivot.y); }}); \
        properties.push_back({"zOffset", PropertyType::Float, [this](){ return std::to_string(zOffset); }, [this](const std::string& value){ zOffset = std::stof(value); }}); \
        return properties; \
    }
