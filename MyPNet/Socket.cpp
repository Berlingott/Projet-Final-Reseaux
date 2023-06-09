//
// Utilisation de socket dans MyPNET.. causait problème, utilisation de socket directement dans les programme avec winsock dans la cible et le server
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


}