//
// Created by Berlingot on 2023-04-20.
//

#ifndef CLIENT___PIRATE_CONNEXION_H
#define CLIENT___PIRATE_CONNEXION_H
#include "../MyPNet/IncludeMe.h"
#include "ConnexionType.h"
#include "GestionPacket.h"
#include "ActiveConnexionVerification.h"

#include <unistd.h>


#include <WinSock2.h> //For win sockets
#include <string> //For std::string
#include <iostream> //For std::cout, std::endl
#include <vector> //for std::vector
#include <memory>


class Connexion {
private:
    //variables
    MyPNet::Socket _socket;
    bool pirateTrouvee = false;
    ConnexionType connexiontype;
    SOCKADDR_IN adresseIP;
    unsigned short port;

    //fonctions
    void INBOUNDconnexion(const char * adresseIP, unsigned short port);
    void OUTBOUNDconnexion(const char * adresseIP, unsigned short port);
    static void TransmitionDePaquetThread();

    GestionPacket gestionpacket;
    static Connexion * connexionptrreference;
    std::vector<std::shared_ptr<ActiveConnexionVerification>> activeconnection;
    bool sendallpack(int ID, char * data, int totalbytes);


    bool listenning();
public:
    //constructeur
    Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg);//INbound
    MyPNet::Socket* getSocket() {return &_socket;};
    SOCKET listenersocket;
};




#endif //CLIENT___PIRATE_CONNEXION_H
