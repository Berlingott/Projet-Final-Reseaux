//
// tout ce qui constitue un packet
// aide a l'envoie et au communication
// est l'objet en lui meme ne sera pas envoyé, mais tout ce que nous devont envoyer est contenu dans cette classe

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
        std::vector<char> bufferMyPNet;

        Packet();

        Packet(char * bufferL, int size); //Will use existing allocated buffer and create packet from it
        Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
        int size;
        char * bufferL;
        std::string read_backstring;
        std::string read_back();
    };
}
#endif //MYPNET_PACKET_H
