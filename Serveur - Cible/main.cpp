#include "iostream"
#include "Client.h"
#include "ParametresGeneraux.h"
#include "../MyPNet/IncludeMe.h"
int main(){ // Main client -- cible
    if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;

            if (socket.ConnectTo(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){
                std::cout <<"Nouvelle connexion établie vers le malfrat!" << std::endl;

                //send paquets
                uint32_t  a, b, c;
                a=4;
                b=6;
                c=9;
                MyPNet::Packet packet;
                packet << std::string ("Bonjour");
                packet << std::string ("hello");

                while(true){
                    MyPNet::PResult result = socket.SendPaquets(packet);
                    if (result != MyPNet::PResult::P_Success){
                        break;
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
}