#include "Running.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCommandLine, int nCommandShow){ // Main client -- cible
    Running::lpArguments = lpCommandLine; // set variable static selon l'entree en arguments

    if (Running::initialisation())	//runs init() and exits file if it installs itself (starts new file)
        return 0;



}