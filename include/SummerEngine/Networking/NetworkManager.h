#pragma once
#include "ITransport.h"
#include <unordered_map>
#include <functional>

using PacketHandler = std::function<void(Packet&)>;

class NetworkManager
{
private:
    std::unordered_map<uint16_t, PacketHandler> callbacks;

public:
    static NetworkManager* singleton;
    ITransport* m_transport;

    NetworkManager();
    ~NetworkManager();

    // Set the backend (e.g., Steam, LAN)
    void SetTransport(ITransport* transport);

    // Call this in your game loop
    void Update();

    // Broadcast/Recieve Data functions
    void BroadcastReliable(Packet packet);
    void BroadcastUnreliable(Packet packet);
    void ReceivePacket(const uint8_t* data, size_t size);

    //Register callbacks
    void RegisterCallback(std::string stringId, PacketHandler handler);

    // Call when closing the game
    void Shutdown();
};