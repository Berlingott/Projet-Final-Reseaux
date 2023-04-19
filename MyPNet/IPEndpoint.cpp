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
        // resoudre le hostna,e
        getaddrinfo()
    }




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