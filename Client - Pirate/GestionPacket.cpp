//
// Created by Berlingot on 2023-04-21.
//

#include "GestionPacket.h"

void GestionPacket::Clear() {
    std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at same time
    std::queue<MyPNet::Packet> empty;
    std::swap(queue_packets, empty); //Clear out packet queue
}

bool GestionPacket::HasPendingPackets() {
    return (queue_packets.size() > 0); //returns true if size > 0 false otherwise
}

void GestionPacket::Append(MyPNet::Packet p) {
    std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at same time
    queue_packets.push(p); //Add packet to queue
}

MyPNet::Packet GestionPacket::Retrieve() {
    std::lock_guard<std::mutex> lock(mutex_packets); //lock mutex so two threads dont access data at same time
    MyPNet::Packet p = queue_packets.front(); //Get packet from front of queue
    queue_packets.pop(); //Remove packet from front of queue
    return p; //Return packet that was removed from the queue
    }
