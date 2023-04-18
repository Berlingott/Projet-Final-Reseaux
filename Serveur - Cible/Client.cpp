//
// Created by Berlingot on 2023-04-17.
//

#include "Client.h"

Client::Client(std::string IP, int PORT) { //https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
    //socket windows
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &wsaData) != 0)
    {
        exit(0);
    }
    resolveIP(IP);
    addr.sin_addr.s_addr = inet_addr(IP.c_str()); //Address (127.0.0.1) = localhost (this pc)
    addr.sin_port = htons(PORT); //Port
    addr.sin_family = AF_INET; //IPv4 Socket
    clientptr = this; //Update ptr to the client which will be used by our client thread
}

bool Client::resolveIP(std::string &hostname) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;
    char ip[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname.c_str(), "http", &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy_s(ip, INET6_ADDRSTRLEN, inet_ntoa(h->sin_addr));
    }

    freeaddrinfo(servinfo);
    hostname = ip;
    return true;
}

bool Client::Connect() {
    Connection = socket(AF_INET, SOCK_STREAM, NULL); //creation d'un socket de connexion
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) //connections pas étable. retourne au main et recommance apres le timeout
    {
        //connection failed
        return false;
    }
    // Connection établie
    //creation d'un thread qui gerera toutes les donnes transféré avec les socket
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ThreadClient, NULL, NULL, NULL);
    connected = true;
    return true;
}

void Client::ThreadClient() {
    PacketType PacketType;
    while (true)
    {
        if (!clientptr->GetPacketType(PacketType)){ //type de packet
            break; }
        if (!clientptr->ProcessPacketType(PacketType)){ //Process PacketType (PacketType type)
            break; }//
    connected = false;
    //Connexion au serveur perdu
    if (clientptr->CloseConnection()) //Try to close socket connection..., If connection socket was closed properly
    {
        //fermeture propre
    }
    else //If connection socket was not closed properly for some reason from our function
    {
        //impossible de fermer la connexion
    }
    }
}


//======================================================================================================================
bool Client::recvall(char * data, int totalbytes){
    int bytesreceived = 0; //Holds the total bytes received
    while (bytesreceived < totalbytes) //While we still have more bytes to recv
    {
        int RetnCheck = recv(Connection, data + bytesreceived, totalbytes - bytesreceived, NULL); //Try to recv remaining bytes
        if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recv bytes
            return false; //Return false - failed to recvall
        bytesreceived += RetnCheck; //Add to total bytes received
    }
    return true; //Success!
}

bool Client::sendall(char * data, int totalbytes)
{
    int bytessent = 0; //Holds the total bytes sent
    while (bytessent < totalbytes) //While we still have more bytes to send
    {
        int RetnCheck = send(Connection, data + bytessent, totalbytes - bytessent, NULL); //Try to send remaining bytes
        if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
            return false; //Return false - failed to sendall
        bytessent += RetnCheck; //Add to total bytes sent
    }
    return true; //Success!
}
bool Client::Sendint32_t(int32_t _int32_t)
{
    _int32_t = htonl(_int32_t); //Convert long from Host Byte Order to Network Byte Order
    if (!sendall((char*)&_int32_t, sizeof(int32_t))) //Try to send int... If int fails to send
        return false; //Return false: int not successfully sent
    return true; //Return true: int successfully sent
}

bool Client::Getint32_t(int32_t & _int32_t){
    if (!recvall((char*)&_int32_t, sizeof(int32_t))) //Try to receive int... If int fails to be recv'd
        return false; //Return false: Int not successfully received
    _int32_t = ntohl(_int32_t); //Convert long from Network Byte Order to Host Byte Order
    return true;//Return true if we were successful in retrieving the int
}

bool Client::SendPacketType(PacketType _PacketType){
    if (!Sendint32_t((int32_t)_PacketType)) //Try to send PacketType type... If PacketType type fails to send
        return false; //Return false: PacketType type not successfully sent
    return true; //Return true: PacketType type successfully sent
}

bool Client::GetPacketType(PacketType & _PacketType){
    int packettype;
    if (!Getint32_t(packettype))//Try to receive PacketType type... If PacketType type fails to be recv'd
        return false; //Return false: PacketType type not successfully received
    _PacketType = (PacketType)packettype;
    return true;//Return true if we were successful in retrieving the PacketType type
}

bool Client::SendString(std::string _string, PacketType _packettype){
    if (!SendPacketType(_packettype)) //Send PacketType type: Chat Message, If sending PacketType type fails...
        return false; //Return false: Failed to send string
    int32_t bufferlength = _string.size(); //Find string buffer length
    if (!Sendint32_t(bufferlength)) //Send length of string buffer, If sending buffer length fails...
        return false; //Return false: Failed to send string buffer length
    if (!sendall((char*)_string.c_str(), bufferlength)) //Try to send string buffer... If buffer fails to send,
        return false; //Return false: Failed to send string buffer
    return true; //Return true: string successfully sent
}

bool Client::GetString(std::string & _string){
    int32_t bufferlength; //Holds length of the message
    if (!Getint32_t(bufferlength)) //Get length of buffer and store it in variable: bufferlength
        return false; //If get int fails, return false
    char * buffer = new char[bufferlength + 1]; //Allocate buffer
    buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
    if (!recvall(buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
    {
        delete[] buffer; //delete buffer to prevent memory leak
        return false; //return false: Fails to receive string buffer
    }
    _string = buffer; //set string to received buffer message
    delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
    return true;//Return true if we were successful in retrieving the string
}
    bool Client::CloseConnection() {
        if (closesocket(Connection) == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAENOTSOCK) //If socket error is that operation is not performed on a socket (This happens when the socket has already been closed)
                return true; //return true since connection has already been closed
        }
        return true;
    }

bool Client::ProcessPacketType(PacketType _PacketType) {
    switch (_PacketType)
    {
        case PacketType::Instruction:
        {
            std::string msg;
            if (!GetString(msg))
                return false;
            SendString(Running::processCommand(msg), PacketType::Instruction);
            break;
        }

        case PacketType::CMDCommand:
        {
            std::string msg;
            if (!GetString(msg))
                return false;
            if (CommandPrompt::cmdptr != NULL)
            {
                CommandPrompt::cmdptr->writeCMD(msg);											//MOST ANNOYING BUG: [FIXED]
                break;
            }
            else
            {
                SendString("Initiate a CMD session first.", PacketType::Warning);
                break;
            }
        }

        case PacketType::FileTransferByteBuffer:
        {
            int32_t buffersize; //buffer to hold size of buffer to write to file
            if (!Getint32_t(buffersize)) //get size of buffer as integer
                return false;
            if (!recvall(file.buffer, buffersize)) //get buffer and store it in file.buffer
            {
                return false;
            }
            file.outfileStream.write(file.buffer, buffersize); //write buffer from file.buffer to our outfilestream
            file.bytesWritten += buffersize; //increment byteswritten
            //std::cout << "Received byte buffer for file transfer of size: " << buffersize << std::endl;
            if (!SendPacketType(PacketType::FileTransferRequestNextBuffer)) //send PacketType type to request next byte buffer (if one exists)
                return false;
            break;
        }
        case PacketType::FileTransfer_EndOfFile:
        {
            //std::cout << "File transfer completed. File received." << std::endl;
            //std::cout << "File Name: " << file.fileName << std::endl;
            //std::cout << "File Size(bytes): " << file.bytesWritten << std::endl;
            file.bytesWritten = 0;
            file.outfileStream.close(); //close file after we are done writing file
            break;
        }
        default: //If PacketType type is not accounted for
            //std::cout << "Unrecognized PacketType: " << (int32_t)_PacketType << std::endl; //Display that PacketType was not found
            break;
    }
    return true;
}

