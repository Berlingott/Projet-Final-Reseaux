//
// Fichier qui permet de faire le transfert de fichier entre la cible et le client
//

#ifndef SERVEUR___CIBLE_UTILITY_H
#define SERVEUR___CIBLE_UTILITY_H

#include <string>

class Utility {
public:
    static bool fileExists(std::string path);
};


#endif
