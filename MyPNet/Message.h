//
// Created by Berlingot on 2023-04-22.
//

#ifndef MYPNET_MESSAGE_H
#define MYPNET_MESSAGE_H

#include "PacketType.h"
#include "Packet.h"
#include <string>

namespace MyPNet{
class Message {
public:
    Message(std::string);
    Packet toPacket(PacketType _packettype); //Converts ChatMessage to packet
private:
    std::string message;
    };
};


#endif //MYPNET_MESSAGE_H
