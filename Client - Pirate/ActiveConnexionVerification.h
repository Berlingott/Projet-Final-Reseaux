//
// Created by Berlingot on 2023-04-21.
//

#ifndef CLIENT___PIRATE_ACTIVECONNEXIONVERIFICATION_H
#define CLIENT___PIRATE_ACTIVECONNEXIONVERIFICATION_H
#include "Connexion.h"
#include "FichierDeTransfer.h"
#include "GestionPacket.h"

class ActiveConnexionVerification {
public:
    ActiveConnexionVerification(SOCKET socket_)
    {
        socket = socket_;
        ActiveConnection = true; //Default to active connection
    }
    bool ActiveConnection; //True if connection is active, false if inactive(due to a disconnect)
    SOCKET socket;
    //file transfer data
    FichierDeTransfer fichier; //Object that contains information about our file that is being sent to the client from this server
    GestionPacket gestionpacket; //Packet Manager for outgoing data for this connection
};


#endif //CLIENT___PIRATE_ACTIVECONNEXIONVERIFICATION_H
