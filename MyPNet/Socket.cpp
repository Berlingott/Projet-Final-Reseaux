//
// Created by Berlingot on 2023-04-19.
//

#include "Socket.h"
#include <assert.h>
namespace MyPNet{

MyPNet::Socket::Socket(MyPNet::IPVersion ipversion, MyPNet::SocketHandle handle)
:ipversion(ipversion), handle(handle)
{
    assert(ipversion == IPVersion::IPv4); //s'assure que L'adresse respecte le format de l'IPv4
}

MyPNet::PResult MyPNet::Socket::CreateSocket() {
    assert(ipversion == IPVersion::IPv4);
    if (handle != INVALID_SOCKET)
    {
        //todo gestion de l'erreur
    }
    //creation du socket
    handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // communication tcp, protocole tcp, -- retourne le socket handle, sinon retournera un handle invalid_socket

    if(handle == INVALID_SOCKET){
        //todo erreur pour le socket invalid et gestion de l'erreur
    }

    if(SetSocketOptions(SocketOptions::TCP_NoDelay, TRUE) != PResult::P_Success){
        //todo gestion d'erreur
    }

    return PResult::P_Success;
}

MyPNet::PResult MyPNet::Socket::CloseSocket() { //fermeture dusoclet

    if(handle == INVALID_SOCKET){
        //todo erreur pour le socket invalid et gestion de l'erreur
    }
    int result = closesocket(handle); // si la fermeture du socket se fait correctement, la fonction closesocket retourne 0
    if(result != 0){
        //todo erreur lors de la fermeture du socket et gestion du problèenme
    }

    handle = INVALID_SOCKET;

    return PResult::P_Success;
}

MyPNet::SocketHandle MyPNet::Socket::GetHandle() {
    return handle;
}

MyPNet::IPVersion MyPNet::Socket::GetIPVersion() {
    return ipversion;
}

MyPNet::PResult Socket::SetSocketOptions(SocketOptions option, WINBOOL value) {
    int result;
    switch (option) {
        case SocketOptions::TCP_NoDelay:
            result = setsockopt(handle, IPPROTO_TCP, TCP_NoDelay,(const char*)&value, sizeof(value));
        default:
            return PResult::P_NotYetImplemented;
    }
    if (result != 0 ){
        int error = WSAGetLastError();
        return PResult();
    }
        return PResult::P_Success;
}

    PResult Socket::Bind(IPEndpoint endpoint) {

        sockaddr_in addr = endpoint.GetSockaddrInIPv4();
        int result = bind(handle, (sockaddr*)(&addr), sizeof(sockaddr_in) );

        if (result){
            //todo gestion d'erreur su le bind cause problème
        }

        //int result = bind(handle,)

        return PResult::P_Success;
    }

    PResult Socket::Listen(IPEndpoint endpoint, int backlog) {
        if(Bind(endpoint) != PResult::P_Success){
            //todo gestion d'erreur lors dubind
        }

        int result = listen(handle, backlog); //attend pour un incoming connections
        //return 0 on success

        if(result != 0){
            //todo gestion d'erreur lors de listen
            return PResult::P_FAILED;
        }

        return PResult::P_Success;
    }

    PResult Socket::AcceptConnection(Socket &outSocket) {
        sockaddr_in addr = {};
        int len = sizeof(sockaddr_in);
        SocketHandle acceptedConnectionHandle = accept(handle, (sockaddr*)(&addr), &len); // fait rien tant que pas de connection établie

        if (acceptedConnectionHandle == INVALID_SOCKET){
            //todo gestion d'erreur
            return PResult::P_FAILED;
        }
        IPEndpoint newConnectionEndpoint((sockaddr*)&addr);
        newConnectionEndpoint.PrintAllInfo();
        outSocket = Socket(IPVersion::IPv4, acceptedConnectionHandle);

        return PResult::P_Success;
    }

    PResult Socket::ConnectTo(IPEndpoint endpoint) {
        sockaddr_in addr = endpoint.GetSockaddrInIPv4();
        int result = connect(handle, (sockaddr *)(&addr), sizeof(sockaddr_in) );

        //si success result = 0 sinon donne un nombte
        if (result !=0){
            //todo gestion d'erreur
            return PResult::P_FAILED;
        }

        return P_Success;
    }

    PResult Socket::SendPaquets(const void *data, int numberOfBytes, int &bytesSent) {
        bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);//last arg c'est un flag
        if (bytesSent == SOCKET_ERROR){
            int error = WSAGetLastError();
            //todo implémentation des erreur
        }

        return PResult::P_Success;
    }

    PResult Socket::ReceivePaquets(void *destination, int numberOfBytes, int &bytesReceived) {
        bytesReceived = recv(handle, (char*)destination, numberOfBytes, NULL);//last arg c'est un flag

        if(bytesReceived == 0){ // connection gracefully closed
            return PResult::P_FAILED;
            //todo a implementer en cas de probleme de connexion lors de l'échange de paquets
        }
        if (bytesReceived == SOCKET_ERROR){
            int error = WSAGetLastError();
            return PResult::P_FAILED;
            //todo a implementer en cas de probleme de connexion lors de l'échange de paquets
        }

        return PResult::P_Success;
    }

    PResult Socket::SendALL(const void *data, int numberOfBytes) {
        int totalBytesSent = 0;
        while (totalBytesSent < numberOfBytes){
            int bytesRemaining = numberOfBytes - totalBytesSent;
            int byteSent = 0;
            char * bufferOffset = (char*)data + totalBytesSent;
            PResult result = SendPaquets(bufferOffset, bytesRemaining, byteSent);
            if (result != PResult::P_Success){
                //todo gestion de probleme
                return PResult::P_FAILED;
            }
            totalBytesSent += byteSent;
        }

        return P_Success;
    }

    PResult Socket::ReceiveALL(void *destination, int numberOfBytes) {
        int totalBytesReceived = 0;
        while (totalBytesReceived < numberOfBytes){
            int bytesRemaining = numberOfBytes - totalBytesReceived;
            int byteReceived = 0;
            char * bufferOffset = (char*)destination + totalBytesReceived;
            PResult result = ReceivePaquets(bufferOffset, bytesRemaining, byteReceived);
            if (result != PResult::P_Success){
                //todo gestion de probleme
                return PResult::P_FAILED;
            }
            totalBytesReceived += byteReceived;
        }

        return P_Success;
    }

    PResult Socket::SendPaquets(Packet &packet) {
        uint32_t encodedPacketSize = htonl(packet.buffer.size());
        PResult result = SendALL(&encodedPacketSize, sizeof(uint32_t));

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }

        result = SendALL(packet.buffer.data(), packet.buffer.size());

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }

        return P_Success;
    }

    PResult Socket::ReceivePaquets(Packet &packet) {
        packet.Clear();
        uint32_t encodedSize = 0;
        PResult result = ReceiveALL(&encodedSize, sizeof(uint32_t));

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }

        uint32_t bufferSize = ntohl(encodedSize);
        packet.buffer.resize(bufferSize);
        result = ReceiveALL(&packet.buffer[0], bufferSize);

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }


        return P_Success;
    }


}