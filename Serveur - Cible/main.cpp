#include <iostream>
#include <Windows.h>
#include <tchar.h>


SERVICE_STATUS        g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = NULL;
HANDLE                g_ServiceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler (DWORD);
DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);

#define SERVICE_NAME  _T("My Sample Service")

int _tmain (int argc, TCHAR *argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[] =
            {
                    {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
                    {NULL, NULL}
            };

    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
    {
        return GetLastError ();
    }

    return 0;
}
