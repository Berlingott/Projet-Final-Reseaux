//
// Created by Berlingot on 2023-04-19.
//

#ifndef MYPNET_SOCKET_H
#define MYPNET_SOCKET_H
#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOptions.h"
#include "IPEndpoint.h"
#include "winsock2.h"
#include "iostream"
namespace MyPNet{
    class Socket {
    private:
        IPVersion ipversion = IPVersion::IPv4; // version poar default
        SocketHandle handle = INVALID_SOCKET;// socket handle par defaut
        PResult SetSocketOptions(SocketOptions option, BOOL value);
    public:
        //Constructeur
        Socket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);

        //fonction
        PResult CreateSocket(); // ouverture d'un socket, retourne l'adresse ip
        PResult CloseSocket();
        PResult Bind(IPEndpoint endpoint);
        PResult Listen(IPEndpoint endpoint, int backlog = 5);
        PResult AcceptConnection(Socket & outSocket);
        PResult ConnectTo(IPEndpoint endpoint);
        PResult SendPaquets(void * data, int numberOfBytes, int & bytesSent);
        PResult ReceivePaquets(void * destination, int numberOfBytes, int & bytesReceived);
        PResult SendALL(void * data, int numberOfBytes);
        PResult ReceiveALL(void * destination, int numberOfBytes);

        //getter
        SocketHandle GetHandle();
        IPVersion   GetIPVersion();
    };
}

#endif //MYPNET_SOCKET_H
