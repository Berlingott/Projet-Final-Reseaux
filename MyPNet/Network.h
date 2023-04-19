//
// Created by Berlingot on 2023-04-18.
//

#ifndef MYPNET_NETWORK_H
#define MYPNET_NETWORK_H
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include "iostream"
namespace MyPNet {
    class Network {
        public:
            static bool Initialize();
            static void Shutdown();
    };
}

#endif //MYPNET_NETWORK_H
