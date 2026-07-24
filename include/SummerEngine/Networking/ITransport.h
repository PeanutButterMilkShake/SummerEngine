#pragma once
#include "Packet.h"

class ITransport
{
public:
    virtual ~ITransport() = default;

    // Standard API functions
    virtual void Init() = 0;
    virtual void RunCallbacks() = 0;
    virtual void Shutdown() = 0;

    // Standardized Networking functions
    virtual void SendToAll(Packet packet, bool reliable) = 0;
    virtual void ReceiveAll() = 0;
};