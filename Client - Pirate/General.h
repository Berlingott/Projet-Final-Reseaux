//
// Cette classe permet de gerer les exécution de console sur windows.
// reference https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
//


#ifndef CLIENT___PIRATE_GENERAL_H
#define CLIENT___PIRATE_GENERAL_H

#include <string>
#include <iostream>
#include <Windows.h>
#include "Connexion.h"

class General {
public:	//variables
    static void outputMsg(std::string message, int msgType);
    static bool processParameter(std::string &command, std::string compCommand);
    static bool cmdMode;
};



#endif //CLIENT___PIRATE_GENERAL_H
