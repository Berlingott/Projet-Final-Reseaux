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
                std::string buffer = "Bonjour, je suis une pauvre victime";

                while(true){
                    uint32_t bufferSize = buffer.size();
                    bufferSize = htonl(bufferSize);

                    int result = socket.SendALL(&bufferSize, sizeof(uint32_t));

                    if(result != MyPNet::PResult::P_Success){
                        break;
                    }

                    result = socket.SendALL(buffer.data(), buffer.size());
                    if(result != MyPNet::PResult::P_Success){
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