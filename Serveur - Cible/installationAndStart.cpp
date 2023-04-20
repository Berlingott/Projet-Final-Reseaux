//
// Created by Berlingot on 2023-04-20.
//

#include "installationAndStart.h"
#include <process.h>
std::string installationAndStart::CurrentPath;			//current path of executable
std::string installationAndStart::installationFolder;		//path of folder it should be installed to
std::string installationAndStart::installationPath;			//full path where executable should be installed to

OperationState installationAndStart::init() {
    CurrentPath = getCurrentPath();
    std::cout << "CurrentPath: " << CurrentPath;

    installationFolder = getInstallFolder();
    std::cout << "InstallFolder: " << installationFolder;

    installationPath = getInstallationPath(installationFolder);
    std::cout << "InstallationPath: " << installationPath;


    return OP_Success;
}

std::string installationAndStart::getInstallFolder() {

    std::string rest;
    if (!(Settings::folderName.empty()))
        rest = "\\" + Settings::folderName;

    std::string concat;
    char* buf = 0;
    size_t sz = 0;
   //int result = _dupenv_s(&buf, &sz, Settings::installLocation.c_str());
    if (const char* env_p = getenv(Settings::installLocation.c_str())) //gets environment variable
        if (env_p != nullptr)
        {
            concat = std::string(env_p) + rest; //concatenates string
            free(buf);
        }

    return concat;
}

std::string installationAndStart::getInstallationPath(std::string instFolder) {
    std::string concat;
    concat = instFolder + "\\" + Settings::fileName;

    return concat;
}

std::string installationAndStart::getCurrentPath() {
    char buffer[MAX_PATH];
    GetModuleFileName(0, buffer, MAX_PATH);

    return std::string(buffer);
}

