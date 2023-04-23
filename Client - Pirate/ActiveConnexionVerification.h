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
    ActiveConnexionVerification(SOCKET socket_){//constructeur inline
        socket = socket_;
        ActiveConnection = true;
    }
    bool ActiveConnection; //True if connection is active, false if inactive(due to a disconnect)
    SOCKET socket;



    FichierDeTransfer fichier;
    GestionPacket gestionpacket;
};


#endif //CLIENT___PIRATE_ACTIVECONNEXIONVERIFICATION_H
