//
// fonction d'envoie de reception/conversion de string
//

#include "connexion.h"

bool Connexion::GetString(std::string &_string) {
    int32_t bufferlengthmessage; //grandeur du message
    if (!Getint32_t(bufferlengthmessage)){ //
        return false;} //
    char * bufferpluslongmessage = new char[bufferlengthmessage + 1]; //
    bufferpluslongmessage[bufferlengthmessage] = '\0'; //
    if (!recvall(bufferpluslongmessage, bufferlengthmessage)){//
        delete[] bufferpluslongmessage;
        return false;
    }
    _string = bufferpluslongmessage;
    delete[] bufferpluslongmessage;
    return true;//retourne la string
}


bool Connexion::SendString(std::string _string, MyPNet::PacketType _packettype) {
    if (!SendPacketType(_packettype)){
        return false;
    }
    int32_t bufferMessagelength = _string.size();
    if (!Sendint32_t(bufferMessagelength)) {
        return false;
    }
    if (!sendall((char*)_string.c_str(), bufferMessagelength)){
        return false;
    }
    return true;
}

bool Connexion::Sendint32_t(int32_t _int32_t) {
    _int32_t = htonl(_int32_t);
    if (!sendall((char *) &_int32_t, sizeof(int32_t))) {
        return false;
    }
    return true;
}

bool Connexion::SendPacketType(MyPNet::PacketType _PacketType) {
    if (!Sendint32_t(_PacketType)) {
        return false;
    }
    return true;
}

bool Connexion::sendall(char *data, int totalbytes) {
    int bytesenvoyee = 0;
    while (bytesenvoyee < totalbytes){
        int RetnCheck = send(Connection, data + bytesenvoyee, totalbytes - bytesenvoyee, NULL);
        if (RetnCheck == SOCKET_ERROR) {
            return false;
        }
        bytesenvoyee += RetnCheck;
    }
    return true;
}



