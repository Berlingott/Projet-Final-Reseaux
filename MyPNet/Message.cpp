//
// Les messages sont dans les packets
//  Les messages sont ce qui seront envoyé par le socket
// la premiere partie sert a identifier le type de packet
// la deuxieme la longueur du reste du packet (longueur total - 2 premiers)
// le reste de tailles variables, contient les instruction/messages

#include <iostream>
#include "Message.h"

namespace MyPNet
{
    Message::Message(std::string msg){
        message = msg;
    }

    Packet Message::toPacket(PacketType _packettype){
        //CONSTITUTION D'UN PACKET

        //----------int32_t----------//----------int32_t----------//----------int32_t  * * *       X N     * * *    int32_t----------//
        //                           //                           //                   * * *       X N     * * *                     //
        //         PacketType        //    Longueur du message N  //     char 0        * * *       X N     * * *    char n           //
        //                           //                           //                   * * *       X N     * * *                     //

        // la premiere partie sert a identifier le type de packet
        // la deuxieme la longueur du reste du packet (longueur total - 2 premiers)
        // le reste de tailles variables, contient les instruction/messages

        //calcul de la grandeur du packet
        const int packetsize = sizeof(int32_t) * 2 + message.size() * sizeof(char);

        // creation d'un buffer pour la construction du message de la grandeur calculé
        char * buffer = new char[packetsize];

        //Convert message size (int32_t) to network byte order

        int messagesize = message.size();

        int32_t messagesize32 = (int32_t)messagesize;


        buffer[0]=_packettype;
        buffer[1]=(int32_t)messagesize32;
        for (int32_t i = 2; i < packetsize ; ++i) {
            buffer[i]=(int32_t)message[i-2];
        }
        //on construit un packet avec le message et le packet type
        MyPNet::Packet nouveaupack(buffer, packetsize);

        return nouveaupack; // retourne le packet
    }


}