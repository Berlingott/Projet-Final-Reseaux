//
// Created by Berlingot on 2023-04-20.
//

#ifndef CLIENT___PIRATE_CONNEXION_H
#define CLIENT___PIRATE_CONNEXION_H
#include "../MyPNet/IncludeMe.h"
#include "ConnexionType.h"
#include <unistd.h>

class Connexion {
private:
    //variables
    MyPNet::Socket socket;
    bool pirateTrouvee = false;
    ConnexionType connexiontype;

    //fonctions
    void INBOUNDconnexion(const char * adresseIP, unsigned short port);
    void OUTBOUNDconnexion(const char * adresseIP, unsigned short port);

public:
    //constructeur
    Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg);//INbound
    MyPNet::Socket* getSocket() {return &socket;}

};


#endif //CLIENT___PIRATE_CONNEXION_H
