#include "ProcessFinder.h"

// Rm = RestartManager.
// TO use Restart manager, you should add Rstrtmgr.lib.
// How to:
// Go to Configuration Properties, C/C++ -> General - write Rstrtmgr.lib in Additional Include Directories.
// Under Linker ->Input, write Rstrtmgr.lib under Additional Dependencies.

void ProcessFinder::print_using_processes(PCWSTR fileName)
{
     DWORD session;
     WCHAR sessionKeySize[CCH_RM_SESSION_KEY + 1] = { 0 };

     DWORD error = RmStartSession(&session, 0, sessionKeySize);
     wprintf(L"RmStartSession return error code %d\n", error);
     if (error != ERROR_SUCCESS)
     {
          //RmEndSession(session);
          return;
     }

     error = RmRegisterResources(session, 1, &fileName, 0, NULL, 0, NULL);
     wprintf(L"RmRegisterResources(%ls) returned error code %d\n", fileName, error);
     if (error != ERROR_SUCCESS)
     {
          RmEndSession(session);
          return;
     }

     DWORD reason;
     UINT i;
     UINT numberProcessInfoNeeded;
     UINT numberProcessInfo = 10;
     RM_PROCESS_INFO rgpi[10];

     error = RmGetList(session, &numberProcessInfoNeeded, &numberProcessInfo, rgpi, &reason);
     wprintf(L"RmGetList returned error code %d\n", error);
     if (error != ERROR_SUCCESS)
     {
          RmEndSession(session);
          return;
     }

     wprintf(L"RmGetList returned %d infos (%d needed)\n\n", numberProcessInfo, numberProcessInfoNeeded);
     for (i = 0; i < numberProcessInfo; ++i)
     {
          wprintf(L"Application Type: %d\n", rgpi[i].ApplicationType);
          wprintf(L"Application Name: %p\n", rgpi[i].strAppName);
          wprintf(L"Process Id: %d\n", rgpi[i].Process.dwProcessId);
          HANDLE handleProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, rgpi[i].Process.dwProcessId);

          if (handleProcess == 0)
          {
               CloseHandle(handleProcess);
               continue;
          }

          FILETIME createTime, exitTime, kernalTime, userTime;
          if (GetProcessTimes(handleProcess, &createTime, &exitTime, &kernalTime, &userTime) &&
               CompareFileTime(&rgpi[i].Process.ProcessStartTime, &createTime) == 0)
          {
               WCHAR path[MAX_PATH];
               DWORD cch = MAX_PATH;
               if (QueryFullProcessImageNameW(handleProcess, 0, path, &cch) && cch <= MAX_PATH)
                    wprintf(L"Application Path: %ls\n", path);
          }

          wprintf(L"\n\n");
          CloseHandle(handleProcess);
     }
}