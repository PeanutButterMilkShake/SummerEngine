#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_UI_UIListLayout_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_UI_UIListLayout_h_27() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"paddingOffset", PropertyType::Vector2, [this](){ return std::to_string(paddingOffset.x) + "," + std::to_string(paddingOffset.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &paddingOffset.x, &paddingOffset.y); }}); \
        properties.push_back({"paddingScale", PropertyType::Vector2, [this](){ return std::to_string(paddingScale.x) + "," + std::to_string(paddingScale.y); }, [this](const std::string& value){ sscanf(value.c_str(), "%f,%f", &paddingScale.x, &paddingScale.y); }}); \
        return properties; \
    }
