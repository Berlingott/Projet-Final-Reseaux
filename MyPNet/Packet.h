//
// Created by Berlingot on 2023-04-19.
//

#ifndef MYPNET_PACKET_H
#define MYPNET_PACKET_H

#include <vector>
#include <winsock2.h>
#include <cstdint>
#include <string>
#include "PacketType.h"

namespace MyPNet{
    class Packet{
    private:
    public:
        Packet(PacketType packettype = PacketType::PT_invalid);
        PacketType GetPacketType();
        void AssignPacketType(PacketType packettype);

        //fonctions
        void Clear();
        void append(const void * data, uint32_t size);

        Packet& operator << (uint32_t data);
        Packet& operator >> (uint32_t & data);

        Packet& operator << (const std::string & data);
        Packet& operator >> (std::string & data);

        //variables

        uint32_t extractionOffset = 0;
        std::vector<char> buffer;
    };
}
#endif //MYPNET_PACKET_H