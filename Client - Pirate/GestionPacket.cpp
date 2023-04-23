#include "GestionPacket.h"

void GestionPacket::Clear() {
    std::lock_guard<std::mutex> lock(mutex_packets);
    std::queue<MyPNet::Packet> empty;
    std::swap(queue_packets, empty);
}


MyPNet::Packet GestionPacket::Retrieve() {
    std::lock_guard<std::mutex> lock(mutex_packets);
    MyPNet::Packet p = queue_packets.front();
    queue_packets.pop();
    return p;
}

bool GestionPacket::PacketEnAttente() {
    return (queue_packets.size() > 0);
}

void GestionPacket::Append(MyPNet::Packet p) {
    std::lock_guard<std::mutex> lock(mutex_packets);
    queue_packets.push(p);
}

