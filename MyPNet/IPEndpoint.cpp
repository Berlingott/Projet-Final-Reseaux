//
// Created by Berlingot on 2023-04-19.
//
#include "IPEndpoint.h"

MyPNet::IPVersion MyPNet::IPEndpoint::get_ipversion() {
    return ipversion;
}

std::string MyPNet::IPEndpoint::get_hostname() {
    return hostname;
}

std::vector<uint8_t> MyPNet::IPEndpoint::get_ip_byte() {
    return ip_bytes;
}

unsigned short MyPNet::IPEndpoint::get_port() {
    return port;
}

std::string MyPNet::IPEndpoint::get_ipstring() {
    return ip_string;
}

MyPNet::IPEndpoint::IPEndpoint(const char *ip, unsigned short port) {
    this->port = port;

    in_addr addr; // stock le ipv4
    int result = inet_pton(AF_INET, ip, &addr);

    if (result==1){
        if (addr.S_un.S_addr != INADDR_NONE){
            hostname = ip;
            ip_string = ip;
            ipversion = IPVersion::IPv4;

            ip_bytes.resize((sizeof(ULONG)));
            memcpy(&ip_bytes[0], &addr.S_un.S_addr, sizeof(ULONG));// destination, source, nombre/size de bite a copié
            return;
        }

        // resoudre le hostname

        addrinfo hints = {}; // hints pour filter les résultat de gataddrifnfo
        hints.ai_family = AF_INET;
        addrinfo * hostinfo = nullptr;
        result = getaddrinfo(ip, NULL, &hints, &hostinfo);

        if (result==0){
            auto * host_addr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
            // host_addr->sin_addr.S_un.S_addr;
            ip_string.resize(16);
            inet_ntop(AF_INET, &host_addr->sin_addr.S_un.S_addr, &ip_string[0], 16);

            hostname = ip;
            ULONG ip_long = host_addr->sin_addr.S_un.S_addr;
            ip_bytes.resize(sizeof(ULONG));

            memcpy(&ip_bytes[0],&ip_long,sizeof(ULONG));

            ipversion = IPVersion::IPv4;

            freeaddrinfo(hostinfo);

            return;
        }
    }




}

sockaddr_in MyPNet::IPEndpoint::GetSockaddrInIPv4() {
    assert(ipversion == IPVersion::IPv4); //s'assure que l'ip fourni est du format ipv4 et non ipv6
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, &ip_bytes[0],sizeof(ULONG));
    addr.sin_port = htons(port); // hosttonetwork


    return addr;
}

/* https://learn.microsoft.com/en-us/windows/win32/api/winsock2/ns-winsock2-in_addr

struct in_addr {
union {
struct {
  u_char s_b1;
  u_char s_b2;
  u_char s_b3;
  u_char s_b4;
} S_un_b;
struct {
  u_short s_w1;
  u_short s_w2;
} S_un_w;
u_long S_addr;
} S_un;
};
 *
 *
*/