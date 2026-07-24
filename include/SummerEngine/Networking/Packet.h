#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <cstring>
#include <type_traits>
#include "Utility.h"

template<typename T>
struct is_packet_safe : std::is_trivially_copyable<T> {};

class Packet
{
public:
    std::vector<uint8_t> data;
    size_t readOffset = 0;

    Packet(std::string stringId); // Constructor implementation stays in .cpp
    Packet(const void* rawData, size_t size); // Constructor implementation stays in .cpp
    
    template<typename T>
    void Write(T value)
    {
        static_assert(is_packet_safe<T>(), "Data must be primitive type or struct with no pointers/containers");
        const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&value);
        data.insert(data.end(), ptr, ptr + sizeof(T));
    }

    template<typename T>
    T Read()
    {
        if (readOffset + sizeof(T) > data.size()) return T();

        T value;
        memcpy(&value, data.data() + readOffset, sizeof(T));
        readOffset += sizeof(T);

        return value;
    }
};