//
// Les messages sont dans les packets
//  Les messages sont ce qui seront envoyé par le socket
// la premiere partie sert a identifier le type de packet
// la deuxieme la longueur du reste du packet (longueur total - 2 premiers)
// le reste de tailles variables, contient les instruction/messages
#ifndef MYPNET_MESSAGE_H
#define MYPNET_MESSAGE_H

#include "PacketType.h"
#include "Packet.h"
#include <string>

namespace MyPNet{
class Message {
public:
    Message(std::string);
    Packet toPacket(PacketType _packettype); //preparation d'un message en paquet
    std::string message;
private:
    };
};


#endif //MYPNET_MESSAGE_H
