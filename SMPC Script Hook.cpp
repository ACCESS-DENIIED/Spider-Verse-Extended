#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#ifndef DISPLAY_EXE_NAME
#define DISPLAY_EXE_NAME "SMPC"
#endif

int getProcId(const char* target)
{
    DWORD pID = 0;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            if (strcmp(pe32.szExeFile, target) == 0)
            {
                pID = pe32.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
    return pID;
}

bool StartGame(const std::string& gamePath)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(NULL, const_cast<char*>(gamePath.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return true;
    }
    else
    {
        std::cout << "Failed to start the game." << std::endl;
        return false;
    }
}

int main()
{
    std::vector<std::string> gameExeNames = {"Spider-Man.exe", "MilesMorales.exe"};
    std::filesystem::path currentDir = std::filesystem::current_path();
    
    std::string displayExeName = DISPLAY_EXE_NAME;
    std::wstring wideDisplayName = std::wstring(displayExeName.begin(), displayExeName.end());
    std::wcout << wideDisplayName << " Script Hook - By ACCESS_DENIIED\n" << std::endl;

    std::string gameExePath;
    const char* process = nullptr;
    int pID = 0;

    // Check for both game executables
    for (const auto& exeName : gameExeNames)
    {
        gameExePath = (currentDir / exeName).string();
        pID = getProcId(exeName.c_str());
        
        if (pID != 0 || std::filesystem::exists(gameExePath))
        {
            process = exeName.c_str();
            break;
        }
    }

    if (!process)
    {
        std::cout << "Spider-Man.exe or MilesMorales.exe not found in this directory. Exiting..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::string scriptFolderPath = "scripts";
    std::filesystem::path scriptsDir = currentDir / scriptFolderPath;

    // Check if scripts folder exists
    if (!std::filesystem::exists(scriptsDir))
    {
        std::cout << "Error: 'scripts' folder not found in the game directory." << std::endl;
        std::cout << "Please create a 'scripts' folder and place your script DLLs inside it." << std::endl;
        std::cout << "Press any key to exit." << std::endl;
        std::cin.get();
        return 1;
    }

    if (pID == 0)
    {
        std::cout << "Game process not found. Attempting to start the game..." << std::endl;

        if (!StartGame(gameExePath))
        {
            std::cout << "Game failed to start. Exiting..." << std::endl;
            std::cin.get();
            return 1;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));

        pID = getProcId(process);
        if (pID != 0)
        {
            std::cout << "Game started successfully.\n" << std::endl;
        }
    }

    if (pID == 0)
    {
        std::cout << "Game process not found. Exiting..." << std::endl;
        std::cin.get();
        return 1;
    }

    int numScriptsFound = 0;
    for (const auto& entry : std::filesystem::directory_iterator(scriptsDir))
    {
        if (entry.is_regular_file())
        {
            numScriptsFound++;

            std::string dllName = entry.path().filename().string();
            std::cout << "Detected: " << dllName << std::endl;
        }
    }

    if (numScriptsFound > 0)
    {
        std::cout << std::endl << "Injecting " << numScriptsFound << " script(s) into " << process << "..." << std::endl;
    }
    else
    {
        std::cout << "No scripts found in the 'scripts' folder." << std::endl;
    }

    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pID);
    if (hProcess)
    {
        int loadedCount = 0;
        for (const auto& entry : std::filesystem::directory_iterator(scriptsDir))
        {
            if (entry.is_regular_file())
            {
                std::string dllPath = entry.path().string();
                char dllFullPath[MAX_PATH];
                strcpy_s(dllFullPath, MAX_PATH, dllPath.c_str());

                LPVOID pszLibFileRemote = VirtualAllocEx(hProcess, nullptr, strlen(dllFullPath) + 1,
                    MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                if (pszLibFileRemote)
                {
                    WriteProcessMemory(hProcess, pszLibFileRemote, dllFullPath, strlen(dllFullPath) + 1, nullptr);
                    HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0,
                        (LPTHREAD_START_ROUTINE)LoadLibraryA,
                        pszLibFileRemote, 0, nullptr);
                    if (hThread)
                    {
                        WaitForSingleObject(hThread, INFINITE);
                        CloseHandle(hThread);
                        loadedCount++;
                    }
                    VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);
                }
            }
        }
        CloseHandle(hProcess);

        if (loadedCount > 0)
        {
            std::cout << "Injected " << loadedCount << " scripts successfully.\n" << std::endl;
        }
        else
        {
            std::cout << "No scripts were injected." << std::endl;
        }
    }

    std::cout << "Script hook operation completed. Press any key to exit." << std::endl;
    std::cin.get();
    return 0;
}