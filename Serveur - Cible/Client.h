//
// Created by Berlingot on 2023-04-17.
//

#ifndef SERVEUR___CIBLE_CLIENT_H
#define SERVEUR___CIBLE_CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#include "Running.h"
#include "FileTransferData.h"

enum class PacketType
{
    Instruction,
    CMDCommand,
    Warning,
    FileTransferRequestFile, //Sent to request a file
    FileTransfer_EndOfFile, //Sent for when file transfer is complete
    FileTransferByteBuffer, //Sent before sending a byte buffer for file transfer
    FileTransferRequestNextBuffer //Sent to request the next buffer for file
};
enum Paquet{
    P_Instruction,
    P_CMDCommand,
    P_Error
};
class Client {
private:
    //variables
    SOCKADDR_IN addr;
    SOCKET Connection;
    FileTransferData file;
    //fonctions
    static void ThreadClient();
    bool GetPacketType(PacketType & _PacketType);
    bool ProcessPacketType(PacketType _PacketType);
    bool recvall(char * data, int totalbytes);
    bool Getint32_t(int32_t & _int32_t);
    bool GetString(std::string & _string);
    bool sendall(char * data, int totalbytes);
    bool Sendint32_t(int32_t _int32_t);
    bool SendPacketType(PacketType _PacketType);

protected:
public:
    //variables
    static Client * clientptr;
    static bool connected;

    //fonctions
    Client(std::string IP, int PORT); // constructeur du client, besoin d'une adresse ip et d'un port
    bool resolveIP(std::string &hostname);	//for DNS
    bool Connect();
    bool CloseConnection();
    bool SendString(std::string _string, PacketType _packettype);



};


#endif //SERVEUR___CIBLE_CLIENT_H
