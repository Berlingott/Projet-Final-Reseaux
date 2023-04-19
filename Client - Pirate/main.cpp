//Client pirate


#include "iostream"
#include "../MyPNet/IncludeMe.h"

int main(){ // Main client -- cible
    if(MyPNet::Network::Initialize()){
        MyPNet::IPEndpoint test("192.168.0.2", 1337);
        if (test.get_ipversion()==MyPNet::IPVersion::IPv4){
            std::cout << "Hostname: " << test.get_hostname() << std::endl;
            std::cout << "IP: " << test.get_ipstring()<<std::endl;
            std::cout << "Port: " << test.get_port() << std::endl;
            std::cout << std::endl;

            std::cout << "Bytes: "<< std::endl;

            for (auto & digit : test.get_ip_byte()) {
                std::cout << int(digit) << std::endl;
            }
        }
        else{
            std::cerr << "Hoho.. il y a un problème avec L'IPv4";
        }
        MyPNet::Socket socket;
        if(socket.CreateSocket() == MyPNet::PResult::P_Success){
            std::cerr << std::endl<< "Création du socket réussi";
            socket.CloseSocket();
        }
    }

    MyPNet::Network::Shutdown(); //ferme l'interface
    return 0;
}