#pragma once

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID include_SummerEngine_Scene_Components_UI_UIText_h

#include <string>
#include <vector>
#include <cstdio>

#define include_SummerEngine_Scene_Components_UI_UIText_h_57() \
public: \
    std::vector<PropertyInfo> GetProperties() override { \
        std::vector<PropertyInfo> properties; \
        properties.push_back({"fontSize", PropertyType::Float, [this](){ return std::to_string(fontSize); }, [this](const std::string& value){ fontSize = std::stof(value); }}); \
        properties.push_back({"fontWeight", PropertyType::Float, [this](){ return std::to_string(fontWeight); }, [this](const std::string& value){ fontWeight = std::stof(value); }}); \
        properties.push_back({"characterPadding", PropertyType::Float, [this](){ return std::to_string(characterPadding); }, [this](const std::string& value){ characterPadding = std::stof(value); }}); \
        properties.push_back({"wordPadding", PropertyType::Float, [this](){ return std::to_string(wordPadding); }, [this](const std::string& value){ wordPadding = std::stof(value); }}); \
        properties.push_back({"linePadding", PropertyType::Float, [this](){ return std::to_string(linePadding); }, [this](const std::string& value){ linePadding = std::stof(value); }}); \
        return properties; \
    }
