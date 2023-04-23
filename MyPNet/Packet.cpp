//
// tout ce qui constitue un packet
// aide a l'envoie et au communication
// est l'objet en lui meme ne sera pas envoyé, mais tout ce que nous devont envoyer est contenu dans cette classe


#include "Packet.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <iostream>

MyPNet::Packet::Packet()
{

}

MyPNet::Packet::Packet(char * _buffer, int _size)
{
    bufferL = _buffer;
    size = _size;
}

void MyPNet::Packet::Clear() {
    bufferMyPNet.resize(sizeof(PacketType));
    AssignPacketType(PacketType::PT_invalid);
    extractionOffset = sizeof(PacketType);
}

void MyPNet::Packet::append(const void *data, uint32_t size) {
    bufferMyPNet.insert(bufferMyPNet.end(), (char*)data, (char*)data+size);
}

MyPNet::Packet &MyPNet::Packet::operator<<(uint32_t data) {
    data = htonl(data);
    append(&data, sizeof(uint32_t));
    return *this;
}

MyPNet::Packet &MyPNet::Packet::operator>>(uint32_t & data) {
    data = *reinterpret_cast<uint32_t*>(&bufferMyPNet[extractionOffset]);
    data = ntohl(data);
    extractionOffset += sizeof (uint32_t);
    return *this;
}

MyPNet::Packet &MyPNet::Packet::operator<<(const std::string & data) {// permet la reception et l'envoie de string avec >> ou << comme std::cout
    *this << (uint32_t)data.size();
    append(data.data(), data.size());
    return *this;

}

MyPNet::Packet &MyPNet::Packet::operator>>(std::string & data){// permet la reception et l'envoie de string avec >> ou << comme std::cout
    data.clear();

    uint32_t stringSize = 0;
    *this >> stringSize;

    data.resize(stringSize);
    data.assign(&bufferMyPNet[extractionOffset], stringSize);
    extractionOffset +=stringSize;

    return *this;
}

MyPNet::Packet::Packet(MyPNet::PacketType packettype) {
    Clear();
    AssignPacketType(packettype);
}

MyPNet::PacketType MyPNet::Packet::GetPacketType() {
    PacketType * packetTypePtr = reinterpret_cast<PacketType*>(&bufferMyPNet[0]);
    return static_cast<PacketType>(ntohs(*packetTypePtr));
}

void MyPNet::Packet::AssignPacketType(MyPNet::PacketType packettype) {
    PacketType * packetTypePtr = reinterpret_cast<PacketType*>(&bufferMyPNet[0]);
    *packetTypePtr = static_cast<PacketType>(htons(packettype));

}




MyPNet::Packet::Packet(const Packet & p) //allocation de l'espace d'un buffer
{
    size = p.size;
    bufferL = new char[size];


    for (int32_t i = 0; i < p.size ; ++i) {
        bufferL[i] = p.bufferL[i];
        std::cout << this->read_back();
    }
    this->read_back();
}

std::string MyPNet::Packet::read_back() { // aide lors du debug
    std::ifstream file(bufferL, std::ios_base::binary);
    std::ostringstream temp;
    temp << file.rdbuf();

    read_backstring = temp.str();
    return temp.str();
}
