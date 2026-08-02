#pragma once
#include <functional>
#include <unordered_map>
#include <cstdint>

template<typename... Args>
class Event;

template<typename... Args>
struct Listener
{
public:
    Event<Args...>* event = nullptr;
    uint32_t id = 0;

    void StopListening()
    {
        if (event)
        {
            event->StopListening(id);
        }
    }
};

template<typename... Args>
class Event
{
public:
    using EventCallback = std::function<void(Args...)>;

    // 1. Standard Listen (for free functions and lambdas)
    Listener<Args...> AddListener(EventCallback callback)
    {
        uint32_t id = m_nextId++;
        m_listeners[id] = callback;

        Listener<Args...> listener;
        listener.event = this;
        listener.id = id;

        return listener;
    }

    template <typename T>
    Listener<Args...> AddListener(T* instance, void (T::*memberFunction)(Args...))
    {
        return AddListener([instance, memberFunction](Args... args) {
            (instance->*memberFunction)(args...);
        });
    }

    void StopListening(uint32_t listenerId)
    {
        m_listeners.erase(listenerId);
    }

    void Broadcast(Args... args)
    {
        for (auto& [id, callback] : m_listeners)
        {
            callback(args...);
        }
    }

    void Clear()
    {
        m_listeners.clear();
    }

private:
    std::unordered_map<uint32_t, EventCallback> m_listeners;
    uint32_t m_nextId = 0;
};