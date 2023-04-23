//
// Created by Berlingot on 2023-04-21.
//

#ifndef CLIENT___PIRATE_GESTIONPACKET_H
#define CLIENT___PIRATE_GESTIONPACKET_H


#include <queue>
#include <mutex>
#include "../MyPNet/IncludeMe.h"



class GestionPacket {
private:
    std::queue<MyPNet::Packet> queue_packets;
    std::mutex mutex_packets;
public:
    void Clear();
    void Append(MyPNet::Packet p);
    bool PacketEnAttente();
    MyPNet::Packet Retrieve();
};


#endif //CLIENT___PIRATE_GESTIONPACKET_H
