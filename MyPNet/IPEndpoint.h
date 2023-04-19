//
// Created by Berlingot on 2023-04-19.
//

#ifndef MYPNET_IPENDPOINT_H
#define MYPNET_IPENDPOINT_H

#include <string>
#include <vector>
#include "IPVersion.h"
#include <ws2tcpip.h>
#include "IPEndpoint.h"

namespace MyPNet{
    class IPEndpoint
    {
    private:
        //variables
        IPVersion ipversion = IPVersion::inconnu ;//inconnu par default
        std::vector<uint8_t> ip_bytes; //uint8_T = meme chose que byte --> vecteur de byte signed integer type with width of exactly 8, 16, 32 and 64 bits respectively
        std::string hostname;
        std::string ip_string;
        unsigned short port;

        //fonctions

    public:
        //constructeur
        IPEndpoint(const char * ip, unsigned short port);
        //getter
        IPVersion get_ipversion();
        std::string get_hostname();
        std::vector<uint8_t> get_ip_byte();
        unsigned short get_port();
        std::string get_ipstring();
    };
}
#endif //MYPNET_IPENDPOINT_H
