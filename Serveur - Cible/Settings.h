//
// Created by Berlingot on 2023-04-20.
//

#ifndef SERVEUR___CIBLE_SETTINGS_H
#define SERVEUR___CIBLE_SETTINGS_H
#include <iostream>
class Settings {
public:
    static std::string fileName;
    static std::string folderName;
    static std::string startupName;
    static std::string logFileName;
    static std::string installLocation;

    static bool installSelf;			//specifies whether the program should install itself
    static bool setStartupSelf;			//specifies whether the program is to be started on system boot
    static bool startOnNextBoot;
    static bool meltSelf;				//specifies whether the installed clone should delete the initial file

    static const char * adresseipPirate;
    static unsigned short portPirate;


};
#endif //SERVEUR___CIBLE_SETTINGS_H
