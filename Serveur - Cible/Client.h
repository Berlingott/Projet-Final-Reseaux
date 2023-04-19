//
// Created by Berlingot on 2023-04-17.
//

#ifndef SERVEUR___CIBLE_CLIENT_H
#define SERVEUR___CIBLE_CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

class Client {
private:
    static bool connected;

protected:
public:
    //Constructeurs
    Client(std::string adresseIPpirate, int port);
    //Variables
    static Client * staticptr_client;
    //Fonctions

};


#endif //SERVEUR___CIBLE_CLIENT_H
