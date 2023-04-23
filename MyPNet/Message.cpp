//
// Created by Berlingot on 2023-04-22.
//

#include "Message.h"

namespace MyPNet
{
    Message::Message(std::string msg)
    {
        message = msg;
    }

    Packet Message::toPacket(PacketType _packettype)
    {//Calculate total size of buffer for packet contents
        const int packetsize = sizeof(int32_t) * 2 + message.size() * sizeof(char);
        //Create buffer big enough to hold all info for message
        char * buffer = new char[packetsize];
        //Convert packet type (int32_t) to network byte order
        int32_t packettype = (int32_t)_packettype;//todo uint32 en uint 32
        //Convert message size (int32_t) to network byte order
        int test = message.size();
        int32_t messagesize = htonl(message.size());
        //Copy Packet Type to first 4 bytes of buffer
        memcpy(buffer, &packettype, sizeof(int32_t));
       //Copy size to next 4 bytes of buffer
        memcpy(buffer + sizeof(int32_t), &messagesize, sizeof(int32_t));
        //Copy message to fill the rest of the buffer
        std::string monmessagetest = message.c_str();//todo string est tu en vie
        memcpy(buffer + sizeof(int32_t) * 2, message.c_str(), message.size() * sizeof(char));
        //Create packet to be returned
        MyPNet::Packet p(buffer, packetsize);
        return p;
    }
}