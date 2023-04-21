//
// Created by Berlingot on 2023-04-21.
//

#ifndef CLIENT___PIRATE_GESTIONPACKET_H
#define CLIENT___PIRATE_GESTIONPACKET_H


#include <queue>
#include <mutex>

class Packet
{
public:
    Packet();
    Packet(char * buffer, int size); //Will use existing allocated buffer and create packet from it
    Packet(const Packet & p); //Will allocate new buffer but copy buffer from packet argument
    int size{};
    char * buffer{};
};



class GestionPacket {
private:
    std::queue<Packet> queue_packets;
    std::mutex mutex_packets;
public:
    void Clear();
    bool HasPendingPackets();
    void Append(Packet p);
    Packet Retrieve();
};


#endif //CLIENT___PIRATE_GESTIONPACKET_H
