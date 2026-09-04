// PropertyRegistry.h
#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "PropertyUI.h"
#include "FloatPropertyUI.h"
#include "BoolPropertyUI.h"
#include "Vector2PropertyUI.h"
#include "Vector3PropertyUI.h"

#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#endif

class PropertyRegistry
{
public:
    PropertyRegistry() = delete;

    template <typename T>
    static void RegisterDrawer(PropertyUI* func)
    {
        GetMap()[typeid(T)] = func;
    }

    static std::string DemangleTypeName(const char* name)
    {
    #ifdef __GNUG__
        int status = -4;
        // abi::__cxa_demangle allocates memory, so we must free it
        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        if (status == 0 && demangled != nullptr)
        {
            std::string result(demangled);
            std::free(demangled);
            return result;
        }
    #endif
        // Fallback if demangling fails or if using MSVC (which is already readable)
        return name;
    }

    // Build UI
    static EngineObject* BuildUI(std::type_index type, const std::string& propertyName, const std::string& propertyValue, std::function<void(const std::string&)> setPropertyValue)
    {
        auto& map = GetMap();
        auto it = map.find(type);
        if (it != map.end())
        {
            return it->second->BuildUI(propertyName, propertyValue, setPropertyValue);
        }

    std::string readableName = DemangleTypeName(type.name());
    printf("No BuildUI function for type: %s\n", readableName.c_str());

        return nullptr;
    }

    static void Init()
    {
        RegisterDrawer<float>(new FloatPropertyUI());
        RegisterDrawer<bool>(new BoolPropertyUI());
        RegisterDrawer<Vector2>(new Vector2PropertyUI());
        RegisterDrawer<Vector3>(new Vector3PropertyUI());
    };

private:
    static std::unordered_map<std::type_index, PropertyUI*>& GetMap()
    {
        static std::unordered_map<std::type_index, PropertyUI*> registryMap;
        return registryMap;
    }
};