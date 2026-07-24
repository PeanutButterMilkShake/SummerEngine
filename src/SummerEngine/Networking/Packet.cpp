#include "Packet.h"

Packet::Packet(std::string stringId)
{
    Write(HashString(stringId.c_str()));
}

Packet::Packet(const void* rawData, size_t size)
{
    data.assign((const uint8_t*)rawData, (const uint8_t*)rawData + size);
}