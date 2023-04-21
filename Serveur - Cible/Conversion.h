//
// Created by Berlingot on 2023-04-20.
//

#ifndef SERVEUR___CIBLE_CONVERSION_H
#define SERVEUR___CIBLE_CONVERSION_H

#include <string>
#include <winsock2.h>
#include <Windows.h>

class Conversion
{
public:
    static LPTSTR convStringToLPTSTR(std::string s);
    static std::wstring convStringToWidestring(const std::string& s);
};


#endif //SERVEUR___CIBLE_CONVERSION_H
