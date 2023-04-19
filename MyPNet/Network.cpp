//
// Created by Berlingot on 2023-04-18.
//

#include "Network.h"


bool MyPNet::Network::Initialize() {
    WSADATA wsadata; //winsock API data structure
    int result = WSAStartup(MAKEWORD(2,2),&wsadata); //MAKEWORD Version 2.2 créé un WORD en concatenant les differentes valeurs
    if(result != 0){
        //WSAStartup non réussi
        //todo prendre en charge l'erreur
        std::cerr << "Echec du lancement de l'API winsock";
        return false;
    }
    if (LOBYTE(wsadata.wVersion) !=2 || HIBYTE(wsadata.wVersion)!=2){
        //WSAStartup non réussi
        //todo prendre en charge l'erreur
        std::cerr << "Erreur lors du chargement de la version de winsock";
        return false;
    }
    std::cerr << "Demarage du Winsock API DATA Structure";

    return true; // tout va bien
}

void MyPNet::Network::Shutdown() {
    WSACleanup();
}
