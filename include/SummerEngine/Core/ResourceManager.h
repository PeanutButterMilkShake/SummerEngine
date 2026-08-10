#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <format>

class Resource;

namespace ResourceManager
{
    // Magic static inline for any resource
    template <typename T>
    inline std::unordered_map<std::string, std::shared_ptr<T>>& GetCache()
    {
        static std::unordered_map<std::string, std::shared_ptr<T>> cache;
        return cache;
    }

    // Get resource from cache, create cache if not existing
    template <typename T, typename... Args>
    inline std::shared_ptr<T> CreateResource(const std::string& key, Args&&... args)
    {
        static_assert(std::is_base_of_v<Resource, T>, "ResourceManager::Get<T>() can only take valid resources");

        auto& cache = GetCache<T>();

        // Attempt to find resource
        auto it = cache.find(key);
        if (it != cache.end())
        {
            return it->second;
        }

        // Create resource if not existing
        std::shared_ptr<T> resource;

        if constexpr (sizeof...(Args) == 0)
        {
            resource = std::make_shared<T>(key);
        }
        else
        {
            resource = std::make_shared<T>(std::forward<Args>(args)...);
        }

        cache[key] = resource;
        return resource;
    }

    // Get resource from cache
    template <typename T, typename... Args>
    inline std::shared_ptr<T> Get(const std::string& key)
    {
        static_assert(std::is_base_of_v<Resource, T>, "ResourceManager::Get<T>() can only take valid resources");

        auto& cache = GetCache<T>();

        auto it = cache.find(key);
        if (it != cache.end())
        {
            return it->second;
        }
        
        return nullptr;
    }
}