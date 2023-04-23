//Client pirate


#include "iostream"
#include "../MyPNet/IncludeMe.h"
#include "Connexion.h"
const char * adresseipPirate = "127.0.0.1";
const unsigned short portPirate = 1337;

bool ProcessPacket(MyPNet::Packet & packet){
    switch (packet.GetPacketType()) {
        case MyPNet::PacketType::PT_ChatMessage: {

            std::string message;
            packet >> message;
            std::cout << message;
            break;
            }
        case MyPNet::PacketType::PT_IntegerArray:{
            uint32_t arraySize = 0;
            packet >> arraySize;
            for (uint32_t i = 0; i < arraySize; i++) {
                uint32_t element=0;
                packet >> element;
                std::cout << element;
            }
            break;
        }
        default:
            return false;
    }
    return true;
}

int main(){ // Main client -- cible
    Connexion connexioninbound(adresseipPirate, portPirate, ConnexionType::INBOUND);

    connexioninbound.ListenForNewConnection();
    connexioninbound.HandleInput();

    system("pause"); // un thread sera en pause ici
    return 0;
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
}*/