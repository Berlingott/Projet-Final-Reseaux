//Client pirate


#include "iostream"
#include "../MyPNet/IncludeMe.h"

int main(){ // Main client -- cible
    if(MyPNet::Network::Initialize()){

        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cerr << std::endl<< "Création du socket réussi";
            if (socket.Bind(MyPNet::IPEndpoint("127.0.0.1",1337))){

                std::cout << "Création du Socket complété";
                
            }
            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
}