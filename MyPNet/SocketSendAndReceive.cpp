//
// Created by Berlingot on 2023-04-20.
//
#include "Socket.h"
namespace MyPNet{
//receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======receive ======
    PResult Socket::ReceivePaquets(Packet &packet) {
        packet.Clear();
        uint16_t encodedSize = 0;
        PResult result = ReceiveALL(&encodedSize, sizeof(uint16_t));

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }

        uint16_t bufferSize = ntohs(encodedSize);

        packet.bufferMyPNet.resize(bufferSize);
        result = ReceiveALL(&packet.bufferMyPNet[0], bufferSize);

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }
        return P_Success;
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





    //send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send ===== send =====


    PResult Socket::SendPaquets(Packet &packet) {
        uint16_t encodedPacketSize = htons(packet.bufferMyPNet.size());
        PResult result = SendALL(&encodedPacketSize, sizeof(uint16_t));

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }

        result = SendALL(packet.bufferMyPNet.data(), packet.bufferMyPNet.size());

        if (result != PResult::P_Success){
            return PResult::P_FAILED;
        }

        return P_Success;
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


    PResult Socket::SendPaquets(const void *data, int numberOfBytes, int &bytesSent) {
        bytesSent = send(handle, (const char*)data, numberOfBytes, NULL);//last arg c'est un flag
        if (bytesSent == SOCKET_ERROR){
            int error = WSAGetLastError();
            //todo implémentation des erreur
        }

        return PResult::P_Success;
    }
}