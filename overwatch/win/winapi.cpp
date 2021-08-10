#include "winapi.hpp"
#include "../macros/utility.hpp"
#include "../exception.hpp"
#include "../macros/global.hpp"

void WinAPI::Process::getProcesses(WinAPI::Process::Types::mapProcessList& plist) {    
    HANDLE hProcessSnap = NULL;
    HANDLE hProcess = NULL;
    PROCESSENTRY32 pe32;
    // LOAD BLACKLIST

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
        throw Exceptions::winapi_error("Invalid hProcessSnap Handle");
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        throw Exceptions::winapi_error("Cannot read Process32First");
    }

    // Skip System, Registry
    Process32Next(hProcessSnap, &pe32);
    if (hProcess == INVALID_HANDLE_VALUE) {
        CloseHandle(hProcessSnap);
        throw Exceptions::winapi_error("Invalid hProcess Handle");
    }
    Process32Next(hProcessSnap, &pe32);
    Process32Next(hProcessSnap, &pe32);

    do {
        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess == INVALID_HANDLE_VALUE)
            CloseHandle(hProcessSnap);
        // TODO : IF PROCESS IN BLACKLIST, SKIP
        plist.insert({ pe32.th32ProcessID, STRING(pe32.szExeFile) + NULL_CHAR });
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return;
}

void WinAPI::Process::getPathProcess(std::filesystem::path& pathProc, const unsigned int& nPID) {
    DWORD PID = nPID;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PID);
    if (hProcess == NULL) {
        throw Exceptions::winapi_error("Invalid hProcess Handle, this might have occurred due to lack of privilege or possibly program has been closed.");
    }

    DWORD value = MAX_WINDOWS_CHAR;
    CHAR_T* buffer = new CHAR_T[MAX_WINDOWS_CHAR];
    if (!QueryFullProcessImageName(hProcess, 0, buffer, &value)) {
        delete[] buffer;
        throw Exceptions::winapi_error("QueryFullProcessImageName Failed");
    }
    pathProc = buffer;

    delete[] buffer;
    CloseHandle(hProcess);
    return;
}