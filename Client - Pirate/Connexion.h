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
#include "General.h"

class Connexion {

private:
    //variables
    MyPNet::Socket _socket;
    bool pirateTrouvee = false;
    ConnexionType connexiontype;
    SOCKADDR_IN adresseIP;//addr
    int addresselen = sizeof(adresseIP);//int addrlen = sizeof(addr);

    unsigned short port;
    int UnusedConnections = 0;

    //fonctions
    void INBOUNDconnexion(const char * adresseIP, unsigned short port);
    void OUTBOUNDconnexion(const char * adresseIP, unsigned short port);
    static void TransmitionDePaquetThread();

    static void ConnexionHandlerThread(int ID); //ClientHandlerThread

    bool ProcessPacket(int ID, MyPNet::PacketType _packettype);

    GestionPacket gestionpacket;
    static Connexion * connexionptrreference;//serverptr
    std::vector<std::shared_ptr<ActiveConnexionVerification>> activeconnection; //connections	std::vector<std::shared_ptr<Connection>> connections;

    bool sendallpack(int ID, char * data, int totalbytes);//sendall
    bool recvallpack(int ID, char * data, int totalbytes);//recvall
    std::mutex connectionMgr_mutex; //mutex for managing connections (used when a client disconnects)
    bool GetString(int ID, std::string & _string);
    bool HandleSendFile(int ID);
    bool SendPacketType(int ID, MyPNet::PacketType _packettype);

    void SendString(int ID, std::basic_string<char> _string, MyPNet::PacketType _packettype);///changement

    static void EcouteThread(); //	static void ListenerThread();
    int currentSessionID;

    bool listenning();

    void handleScript(std::string script);

public:
    //constructeur
    Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg);//INbound
    MyPNet::Socket* getSocket() {return &_socket;};
    SOCKET listenersocket;//sListen
    void ListenForNewConnection();
    bool GetPacketType(int ID, MyPNet::PacketType & _packettype);
    bool Sendint32_t(int ID, int32_t _int32_t);
    bool Getint32_t(int ID, int32_t & _int32_t);
    void DisconnectClient(int ID); //Called to properly disconnect and clean up a client (if possible)
    void HandleInput();

};




#endif //CLIENT___PIRATE_CONNEXION_H
