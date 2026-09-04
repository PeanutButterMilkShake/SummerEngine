#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_UI_UIScrollView_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_UI_UIScrollView_h_24() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"scrollViewSize", PropertyType::Vector2, [this](){ return std::to_string(scrollViewSize.x) + "," + std::to_string(scrollViewSize.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &scrollViewSize.x, &scrollViewSize.y); }}); \
        properties.push_back({"scrollSpeed", PropertyType::Float, [this](){ return std::to_string(scrollSpeed); }, [this](const std::string& value){ scrollSpeed = std::stof(value); }}); \
        return properties; \
    }
