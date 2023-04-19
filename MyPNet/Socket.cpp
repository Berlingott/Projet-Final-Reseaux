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


}