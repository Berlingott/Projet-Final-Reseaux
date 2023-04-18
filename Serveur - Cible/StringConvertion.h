//
// Created by Berlingot on 2023-04-17.
//

#ifndef SERVEUR___CIBLE_STRINGCONVERTION_H
#define SERVEUR___CIBLE_STRINGCONVERTION_H

#include <string>
#include <Windows.h>

class StringConvertion {
public:
    static LPTSTR convStringToLPTSTR(std::string s);
    static std::wstring convStringToWidestring(const std::string& s);
};


#endif //SERVEUR___CIBLE_STRINGCONVERTION_H
