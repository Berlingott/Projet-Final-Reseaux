//Client pirate


#include "iostream"
#include "../MyPNet/IncludeMe.h"

int main(){ // Main client -- cible
    if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cout << std::endl<< "Création du socket réussi"<<std::endl;
            if (socket.Listen(MyPNet::IPEndpoint("127.0.0.1",1337))==MyPNet::PResult::P_Success){

                std::cout << "Écoute du Socket complété"<<std::endl;

            }
            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
}