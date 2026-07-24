#pragma once
#include <functional>
#include <unordered_map>
#include <cstdint>

struct Listener
{
    Event<>* event;
    uint32_t id;

    void StopListening()
    {
        event->StopListening(id);
    }
};

template<typename... Args>
class Event
{
public:
    using EventCallback = std::function<void(Args...)>;

    // 1. Standard Listen (for free functions and lambdas)
    uint32_t Listen(EventCallback callback)
    {
        uint32_t id = m_nextId++;
        m_listeners[id] = callback;

        Listener listener;
        listener.event = this;
        listener.id = id

        return listener;
    }

    template <typename T>
    uint32_t Listen(T* instance, void (T::*memberFunction)(Args...))
    {
        return Listen([instance, memberFunction](Args... args) {
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