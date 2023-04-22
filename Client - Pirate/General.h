//
// Created by Berlingot on 2023-04-22.
//

#ifndef CLIENT___PIRATE_GENERAL_H
#define CLIENT___PIRATE_GENERAL_H

#include <string>
#include <iostream>
#include <Windows.h>
#include "Connexion.h"

class General
{
public:	//functions
    static void outputMsg(std::string message, int msgType);
    static bool processParameter(std::string &command, std::string compCommand);
public:	//variables
    static bool cmdMode;
};



#endif //CLIENT___PIRATE_GENERAL_H
