//
// Created by Berlingot on 2023-04-19.
//

#ifndef MYPNET_PACKET_H
#define MYPNET_PACKET_H

#include <vector>
#include <winsock2.h>
#include <cstdint>
#include <string>

namespace MyPNet{
    class Packet{
    private:
    public:
        //fonctions
        void Clear();
        void append(const void * data, uint32_t size);

        Packet& operator << (uint32_t data);
        Packet& operator >> (uint32_t & data);

        Packet& operator << (const std::string & data);
        Packet& operator >> (std::string & data);

        //variables
        std::vector<char> buffer;
        uint32_t extractionOffset = 0;
    };
}
#endif //MYPNET_PACKET_H
