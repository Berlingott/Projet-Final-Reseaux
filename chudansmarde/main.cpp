#include <iostream>
#include <Windows.h>
enum Packettype : uint32_t {
    instruction,
    asdasdasasd
};

using namespace std;

int main() {

        std::string message = "bofofofofofofofofofofofofo";
        Packettype _packettype = asdasdasasd;

        const int packetsize = sizeof(int32_t) * 2 + message.size() * sizeof(char);

        char * buffer = new char[packetsize];




    int32_t packettype = (int32_t)_packettype;//todo uint32 en uint 32

    int messagesize = message.size();
    int32_t messagesize32 = (int32_t)messagesize;

            buffer[0]=packettype;
     buffer[1]=messagesize32;

    for (::uint32_t i = 2; i < packetsize ; ++i) {
        buffer[i]=message[i-2];
    }


        int sz = sizeof(int32_t);

    std::cout << memcpy(buffer, &packettype, sizeof(int32_t));
    std::cout << memcpy(buffer + sizeof(int32_t), &messagesize32, sizeof(int32_t));
    std::cout << memcpy(buffer + sizeof(int32_t) * 2, message.c_str(), message.size() * sizeof(char));



    system("pause");

}
