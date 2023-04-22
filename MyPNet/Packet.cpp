//
// Created by Berlingot on 2023-04-19.
//
#include "Packet.h"
#include <Windows.h>

void MyPNet::Packet::Clear() {
    buffer.resize(sizeof(PacketType));
    AssignPacketType(PacketType::PT_invalid);
    extractionOffset = sizeof(PacketType);
}

void MyPNet::Packet::append(const void *data, uint32_t size) {
    buffer.insert(buffer.end(), (char*)data, (char*)data+size);
}

MyPNet::Packet &MyPNet::Packet::operator<<(uint32_t data) {
    data = htonl(data);
    append(&data, sizeof(uint32_t));
    return *this;
}

MyPNet::Packet &MyPNet::Packet::operator>>(uint32_t & data) {
    data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
    data = ntohl(data);
    extractionOffset += sizeof (uint32_t);
    return *this;
}

MyPNet::Packet &MyPNet::Packet::operator<<(const std::string & data) {
    *this << (uint32_t)data.size();
    append(data.data(), data.size());
    return *this;

}

MyPNet::Packet &MyPNet::Packet::operator>>(std::string & data){
    data.clear();

    uint32_t stringSize = 0;
    *this >> stringSize;

    data.resize(stringSize);
    data.assign(&buffer[extractionOffset], stringSize);
    extractionOffset +=stringSize;

    return *this;
}

MyPNet::Packet::Packet(MyPNet::PacketType packettype) {
    Clear();
    AssignPacketType(packettype);
}

MyPNet::PacketType MyPNet::Packet::GetPacketType() {
    PacketType * packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
    return static_cast<PacketType>(ntohs(*packetTypePtr));
}

void MyPNet::Packet::AssignPacketType(MyPNet::PacketType packettype) {
    PacketType * packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
    *packetTypePtr = static_cast<PacketType>(htons(packettype));

}


MyPNet::Packet::Packet()
{

}

MyPNet::Packet::Packet(char * _buffer, int _size)
{
    bufferL = _buffer;
    size = _size;
}

MyPNet::Packet::Packet(const Packet & p) //Allocate new block for buffer
{
    size = p.size;
    bufferL = new char[size];
    memcpy(bufferL, &p.buffer, size);
}