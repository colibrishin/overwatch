#include "winapi.hpp"
#include "../macros/utility.hpp"
#include "../macros/global.hpp"

void WinAPI::Process::getProcesses(WinAPI::Process::Types::mapProcessList& plist) {    
    HANDLE hProcessSnap = NULL;
    HANDLE hProcess = NULL;
    PROCESSENTRY32 pe32;
    // LOAD BLACKLIST

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        //TODO
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return; // TODO
    }

    // Skip System, Registry
    Process32Next(hProcessSnap, &pe32);
    if (hProcess == INVALID_HANDLE_VALUE) {
        CloseHandle(hProcessSnap);
        return; // TODO
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

    return; // TODO
}

void WinAPI::Process::getPathProcess(std::filesystem::path& pathProc, const unsigned int& nPID){
    DWORD PID = nPID;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, false, PID);

    DWORD value = MAX_PATH;
    CHAR buffer[MAX_PATH];
    if (!QueryFullProcessImageName(hProcess, 0, buffer, &value))
        return; // TODO
    pathProc = buffer;

    return; // TODO
}

std::string WinAPI::Process::Utility::getErrorMessage(unsigned short code) {
    std::string errstr = "Error : ";

    //code = code ^ Exception::Constants::RTN_EXIT_FAILED;
    switch (code) {
    case Constants::Returns::HANDLE_ACQUIRE_FAILED:
        errstr.insert(errstr.back(), Constants::STR_ERR_HANDLE_ACQUIRE_FAILED);
        break;
    case Constants::Returns::PROCESS32FIRSTERROR:
        errstr.insert(errstr.back(), Constants::STR_ERR_PROCESS32FIRSTERROR);
        break;
    case Constants::Returns::HPROCESS_NULL:
        errstr.insert(errstr.back(), Constants::STR_ERR_HPROCESS_NULL);
        break;
    case Constants::Returns::QUERY_PROCESS_IMAGE_FAILED:
        errstr.insert(errstr.back(), Constants::STR_ERR_QUERY_PROCESS_IMAGE_FAILED);
        break;
    default:
        errstr.insert(errstr.back(), "Unknown Error");
        break;
    }

    return errstr;
}