#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_Light_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_Light_h_25() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"strength", PropertyType::Float, [this](){ return std::to_string(strength); }, [this](const std::string& value){ strength = std::stof(value); }}); \
        return properties; \
    }
