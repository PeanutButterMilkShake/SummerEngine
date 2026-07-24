#include "NetworkManager.h"

NetworkManager* NetworkManager::singleton;

NetworkManager::NetworkManager() : m_transport(nullptr) 
{
    singleton = this;
}

NetworkManager::~NetworkManager()
{
    Shutdown();
}

void NetworkManager::SetTransport(ITransport* transport)
{
    m_transport = transport;
    if (m_transport)
    {
        m_transport->Init();
    }
}

void NetworkManager::Update()
{
    if (!m_transport) return;

    m_transport->RunCallbacks();
    m_transport->ReceiveAll(); // Processes incoming packets
}

void NetworkManager::BroadcastReliable(Packet packet)
{
    if (m_transport)
    {
        m_transport->SendToAll(packet, true);
    }
}

void NetworkManager::BroadcastUnreliable(Packet packet)
{
    if (m_transport)
    {
        m_transport->SendToAll(packet, false);
    }
}

void NetworkManager::ReceivePacket(const uint8_t* data, size_t size)
{
    Packet buffer(data, size);
    uint32_t packetId = buffer.Read<uint32_t>();

    auto it = callbacks.find(packetId);
    if (it != callbacks.end())
    {
        it->second(buffer);
    }
}

void NetworkManager::RegisterCallback(std::string stringId, PacketHandler handler)
{
        uint32_t id = HashString(stringId.c_str());
        callbacks[id] = handler;
}

void NetworkManager::Shutdown()
{
    if (m_transport)
    {
        m_transport->Shutdown();
        m_transport = nullptr;
    }
}