//
// Created by Berlingot on 2023-04-17.
//

#ifndef PROJET_FINAL_RESEAUX_RUNNING_H
#define PROJET_FINAL_RESEAUX_RUNNING_H

#include <Windows.h>
#include <Winbase.h>
#include <string>

#include <Windows.h>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <thread>
class Running {
private:

protected:
public:
    //Variables
    static LPTSTR lpArguments;
    static std::string currentPath;			//Arborescence de fichier présente
    static std::string installFolder;		//Arborescence ou sera installé le fichier
    static std::string installPath;			//Arborescence ou se trouve l'executable
    static bool installing;			//bool - en train d'installer ou pas

    //Fonctions d'initialisation
    static bool initialisation(); //initialisation, lancer au démarage de l'application
    static std::string getCurrentPath();
    static std::string getInstallFolder();
    static std::string getInstallPath(std::string instFolder);

    //Autres Fonctions
};


#endif //PROJET_FINAL_RESEAUX_RUNNING_H
