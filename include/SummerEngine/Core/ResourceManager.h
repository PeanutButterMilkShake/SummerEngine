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
    template <typename T>
    inline std::shared_ptr<T> GetResource(const std::string& key)
    {
        static_assert(std::is_base_of_v<Resource, T>, "ResourceManager::GetResource<T>() can only take valid resources");

        auto& cache = GetCache<T>();

        // Attempt to find resource
        auto it = cache.find(key);
        if (it != cache.end())
        {
            return it->second;
        }

        return nullptr;
    }

    // Add resource to cache
    template <typename T>
    inline std::shared_ptr<T> AddToCache(const std::string& key, std::shared_ptr<T> resource)
    {
        static_assert(std::is_base_of_v<Resource, T>, "ResourceManager::AddToCache<T>() can only take valid resources");

        auto& cache = GetCache<T>();

        // Attempt to find resource
        auto it = cache.find(key);
        if (it != cache.end())
        {
            assert("Cache already has resource with key: " + key + "\n");
        }

        cache[key] = resource;
        return resource;
    }
}