#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <thread>
#include <vector>

// Function pointer declarations for amd_ags_x64.dll
typedef int (WINAPI *agsInitialize_t)(int, void*, void*, void*);
typedef int (WINAPI *agsDeInitialize_t)(void*);
typedef int (WINAPI *agsCheckDriverVersion_t)(void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_BeginUAVOverlap_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_CreateBuffer_t)(void*, void*, void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_CreateDevice_t)(void*, void*, void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_CreateTexture1D_t)(void*, void*, void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_CreateTexture2D_t)(void*, void*, void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_CreateTexture3D_t)(void*, void*, void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_DestroyDevice_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_EndUAVOverlap_t)(void*);
typedef int (WINAPI *agsGetVersionNumber_t)(void);
typedef int (WINAPI *agsSetDisplayMode_t)(void*, void*);
typedef int (WINAPI *agsDriverExtensionsDX11_GetMaxClipRects_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_IASetPrimitiveTopology_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_MultiDrawInstancedIndirect_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_NotifyResourceBeginAllAccess_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_NotifyResourceEndAllAccess_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_NotifyResourceEndWrites_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_NumPendingAsyncCompileJobs_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_SetClipRects_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_SetDepthBounds_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_SetDiskShaderCacheEnabled_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_SetViewBroadcastMasks_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX11_WriteBreadcrumb_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_CreateDevice_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_CreateFromDevice_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_Destroy_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_DestroyDevice_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_PopMarker_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_PushMarker_t)(void*);
typedef int (WINAPI *agsDriverExtensionsDX12_SetMarker_t)(void*);

// Global function pointers
agsInitialize_t pagsInitialize = NULL;
agsDeInitialize_t pagsDeInitialize = NULL;
agsCheckDriverVersion_t pagsCheckDriverVersion = NULL;
agsDriverExtensionsDX11_BeginUAVOverlap_t pagsDriverExtensionsDX11_BeginUAVOverlap = NULL;
agsDriverExtensionsDX11_CreateBuffer_t pagsDriverExtensionsDX11_CreateBuffer = NULL;
agsDriverExtensionsDX11_CreateDevice_t pagsDriverExtensionsDX11_CreateDevice = NULL;
agsDriverExtensionsDX11_CreateTexture1D_t pagsDriverExtensionsDX11_CreateTexture1D = NULL;
agsDriverExtensionsDX11_CreateTexture2D_t pagsDriverExtensionsDX11_CreateTexture2D = NULL;
agsDriverExtensionsDX11_CreateTexture3D_t pagsDriverExtensionsDX11_CreateTexture3D = NULL;
agsDriverExtensionsDX11_DestroyDevice_t pagsDriverExtensionsDX11_DestroyDevice = NULL;
agsDriverExtensionsDX11_EndUAVOverlap_t pagsDriverExtensionsDX11_EndUAVOverlap = NULL;
agsGetVersionNumber_t pagsGetVersionNumber = NULL;
agsSetDisplayMode_t pagsSetDisplayMode = NULL;
agsDriverExtensionsDX11_GetMaxClipRects_t pagsDriverExtensionsDX11_GetMaxClipRects = NULL;
agsDriverExtensionsDX11_IASetPrimitiveTopology_t pagsDriverExtensionsDX11_IASetPrimitiveTopology = NULL;
agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect_t pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect = NULL;
agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect_t pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect = NULL;
agsDriverExtensionsDX11_MultiDrawInstancedIndirect_t pagsDriverExtensionsDX11_MultiDrawInstancedIndirect = NULL;
agsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect_t pagsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect = NULL;
agsDriverExtensionsDX11_NotifyResourceBeginAllAccess_t pagsDriverExtensionsDX11_NotifyResourceBeginAllAccess = NULL;
agsDriverExtensionsDX11_NotifyResourceEndAllAccess_t pagsDriverExtensionsDX11_NotifyResourceEndAllAccess = NULL;
agsDriverExtensionsDX11_NotifyResourceEndWrites_t pagsDriverExtensionsDX11_NotifyResourceEndWrites = NULL;
agsDriverExtensionsDX11_NumPendingAsyncCompileJobs_t pagsDriverExtensionsDX11_NumPendingAsyncCompileJobs = NULL;
agsDriverExtensionsDX11_SetClipRects_t pagsDriverExtensionsDX11_SetClipRects = NULL;
agsDriverExtensionsDX11_SetDepthBounds_t pagsDriverExtensionsDX11_SetDepthBounds = NULL;
agsDriverExtensionsDX11_SetDiskShaderCacheEnabled_t pagsDriverExtensionsDX11_SetDiskShaderCacheEnabled = NULL;
agsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount_t pagsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount = NULL;
agsDriverExtensionsDX11_SetViewBroadcastMasks_t pagsDriverExtensionsDX11_SetViewBroadcastMasks = NULL;
agsDriverExtensionsDX11_WriteBreadcrumb_t pagsDriverExtensionsDX11_WriteBreadcrumb = NULL;
agsDriverExtensionsDX12_CreateDevice_t pagsDriverExtensionsDX12_CreateDevice = NULL;
agsDriverExtensionsDX12_CreateFromDevice_t pagsDriverExtensionsDX12_CreateFromDevice = NULL;
agsDriverExtensionsDX12_Destroy_t pagsDriverExtensionsDX12_Destroy = NULL;
agsDriverExtensionsDX12_DestroyDevice_t pagsDriverExtensionsDX12_DestroyDevice = NULL;
agsDriverExtensionsDX12_PopMarker_t pagsDriverExtensionsDX12_PopMarker = NULL;
agsDriverExtensionsDX12_PushMarker_t pagsDriverExtensionsDX12_PushMarker = NULL;
agsDriverExtensionsDX12_SetMarker_t pagsDriverExtensionsDX12_SetMarker = NULL;

// Handle to the real amd_ags_x64.dll
HMODULE hOriginalDll = NULL;

// Function to log messages
void Log(const std::string& message) {
    std::ofstream logFile("C:\\SMPC_Script_Hook.log", std::ios::app);
    logFile << message << std::endl;
    logFile.close();
}

// Function to load the original DLL and get function pointers
void LoadOriginalDll() {
    std::string originalDllPath = "amd_ags_x64_orig.dll";
    
    hOriginalDll = LoadLibraryA(originalDllPath.c_str());
    if (hOriginalDll) {
        pagsInitialize = (agsInitialize_t)GetProcAddress(hOriginalDll, "agsInitialize");
        pagsDeInitialize = (agsDeInitialize_t)GetProcAddress(hOriginalDll, "agsDeInitialize");
        pagsCheckDriverVersion = (agsCheckDriverVersion_t)GetProcAddress(hOriginalDll, "agsCheckDriverVersion");
        pagsDriverExtensionsDX11_BeginUAVOverlap = (agsDriverExtensionsDX11_BeginUAVOverlap_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_BeginUAVOverlap");
        pagsDriverExtensionsDX11_CreateBuffer = (agsDriverExtensionsDX11_CreateBuffer_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_CreateBuffer");
        pagsDriverExtensionsDX11_CreateDevice = (agsDriverExtensionsDX11_CreateDevice_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_CreateDevice");
        pagsDriverExtensionsDX11_CreateTexture1D = (agsDriverExtensionsDX11_CreateTexture1D_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_CreateTexture1D");
        pagsDriverExtensionsDX11_CreateTexture2D = (agsDriverExtensionsDX11_CreateTexture2D_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_CreateTexture2D");
        pagsDriverExtensionsDX11_CreateTexture3D = (agsDriverExtensionsDX11_CreateTexture3D_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_CreateTexture3D");
        pagsDriverExtensionsDX11_DestroyDevice = (agsDriverExtensionsDX11_DestroyDevice_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_DestroyDevice");
        pagsDriverExtensionsDX11_EndUAVOverlap = (agsDriverExtensionsDX11_EndUAVOverlap_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_EndUAVOverlap");
        pagsGetVersionNumber = (agsGetVersionNumber_t)GetProcAddress(hOriginalDll, "agsGetVersionNumber");
        pagsSetDisplayMode = (agsSetDisplayMode_t)GetProcAddress(hOriginalDll, "agsSetDisplayMode");
        pagsDriverExtensionsDX11_GetMaxClipRects = (agsDriverExtensionsDX11_GetMaxClipRects_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_GetMaxClipRects");
        pagsDriverExtensionsDX11_IASetPrimitiveTopology = (agsDriverExtensionsDX11_IASetPrimitiveTopology_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_IASetPrimitiveTopology");
        pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect = (agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect");
        pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect = (agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect");
        pagsDriverExtensionsDX11_MultiDrawInstancedIndirect = (agsDriverExtensionsDX11_MultiDrawInstancedIndirect_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_MultiDrawInstancedIndirect");
        pagsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect = (agsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect");
        pagsDriverExtensionsDX11_NotifyResourceBeginAllAccess = (agsDriverExtensionsDX11_NotifyResourceBeginAllAccess_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_NotifyResourceBeginAllAccess");
        pagsDriverExtensionsDX11_NotifyResourceEndAllAccess = (agsDriverExtensionsDX11_NotifyResourceEndAllAccess_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_NotifyResourceEndAllAccess");
        pagsDriverExtensionsDX11_NotifyResourceEndWrites = (agsDriverExtensionsDX11_NotifyResourceEndWrites_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_NotifyResourceEndWrites");
        pagsDriverExtensionsDX11_NumPendingAsyncCompileJobs = (agsDriverExtensionsDX11_NumPendingAsyncCompileJobs_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_NumPendingAsyncCompileJobs");
        pagsDriverExtensionsDX11_SetClipRects = (agsDriverExtensionsDX11_SetClipRects_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_SetClipRects");
        pagsDriverExtensionsDX11_SetDepthBounds = (agsDriverExtensionsDX11_SetDepthBounds_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_SetDepthBounds");
        pagsDriverExtensionsDX11_SetDiskShaderCacheEnabled = (agsDriverExtensionsDX11_SetDiskShaderCacheEnabled_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_SetDiskShaderCacheEnabled");
        pagsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount = (agsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount");
        pagsDriverExtensionsDX11_SetViewBroadcastMasks = (agsDriverExtensionsDX11_SetViewBroadcastMasks_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_SetViewBroadcastMasks");
        pagsDriverExtensionsDX11_WriteBreadcrumb = (agsDriverExtensionsDX11_WriteBreadcrumb_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX11_WriteBreadcrumb");
        pagsDriverExtensionsDX12_CreateDevice = (agsDriverExtensionsDX12_CreateDevice_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_CreateDevice");
        pagsDriverExtensionsDX12_CreateFromDevice = (agsDriverExtensionsDX12_CreateFromDevice_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_CreateFromDevice");
        pagsDriverExtensionsDX12_Destroy = (agsDriverExtensionsDX12_Destroy_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_Destroy");
        pagsDriverExtensionsDX12_DestroyDevice = (agsDriverExtensionsDX12_DestroyDevice_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_DestroyDevice");
        pagsDriverExtensionsDX12_PopMarker = (agsDriverExtensionsDX12_PopMarker_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_PopMarker");
        pagsDriverExtensionsDX12_PushMarker = (agsDriverExtensionsDX12_PushMarker_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_PushMarker");
        pagsDriverExtensionsDX12_SetMarker = (agsDriverExtensionsDX12_SetMarker_t)GetProcAddress(hOriginalDll, "agsDriverExtensionsDX12_SetMarker");
    } else {
        Log("Failed to load original amd_ags_x64_orig.dll");
    }
}

// Function to inject DLLs from the "scripts" folder
void InjectScripts() {
    std::filesystem::path currentDir = std::filesystem::current_path();
    std::string scriptFolderPath = "scripts";
    std::filesystem::path scriptsDir = currentDir / scriptFolderPath;

    if (!std::filesystem::exists(scriptsDir)) {
        Log("Error: 'scripts' folder not found in the game directory.");
        return;
    }

    DWORD pID = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (hProcess) {
        for (const auto& entry : std::filesystem::directory_iterator(scriptsDir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dll") {
                std::string dllPath = entry.path().string();
                HMODULE hModule = LoadLibraryA(dllPath.c_str());
                if (hModule) {
                    Log("Loaded: " + dllPath);
                } else {
                    Log("Failed to load: " + dllPath);
                }
            }
        }
        CloseHandle(hProcess);
    }
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        LoadOriginalDll();
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InjectScripts, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
        if (hOriginalDll)
            FreeLibrary(hOriginalDll);
        break;
    }
    return TRUE;
}

// Proxy functions
extern "C" {
    __declspec(dllexport) int WINAPI agsInitialize(int param1, void* param2, void* param3, void* param4) {
        return pagsInitialize ? pagsInitialize(param1, param2, param3, param4) : 0;
    }

    __declspec(dllexport) int WINAPI agsDeInitialize(void* param) {
        return pagsDeInitialize ? pagsDeInitialize(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsCheckDriverVersion(void* param1, void* param2) {
        return pagsCheckDriverVersion ? pagsCheckDriverVersion(param1, param2) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_BeginUAVOverlap(void* param) {
        return pagsDriverExtensionsDX11_BeginUAVOverlap ? pagsDriverExtensionsDX11_BeginUAVOverlap(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_CreateBuffer(void* param1, void* param2, void* param3, void* param4) {
        return pagsDriverExtensionsDX11_CreateBuffer ? pagsDriverExtensionsDX11_CreateBuffer(param1, param2, param3, param4) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_CreateDevice(void* param1, void* param2, void* param3, void* param4) {
        return pagsDriverExtensionsDX11_CreateDevice ? pagsDriverExtensionsDX11_CreateDevice(param1, param2, param3, param4) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_CreateTexture1D(void* param1, void* param2, void* param3, void* param4) {
        return pagsDriverExtensionsDX11_CreateTexture1D ? pagsDriverExtensionsDX11_CreateTexture1D(param1, param2, param3, param4) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_CreateTexture2D(void* param1, void* param2, void* param3, void* param4) {
        return pagsDriverExtensionsDX11_CreateTexture2D ? pagsDriverExtensionsDX11_CreateTexture2D(param1, param2, param3, param4) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_CreateTexture3D(void* param1, void* param2, void* param3, void* param4) {
        return pagsDriverExtensionsDX11_CreateTexture3D ? pagsDriverExtensionsDX11_CreateTexture3D(param1, param2, param3, param4) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_DestroyDevice(void* param) {
        return pagsDriverExtensionsDX11_DestroyDevice ? pagsDriverExtensionsDX11_DestroyDevice(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_EndUAVOverlap(void* param) {
        return pagsDriverExtensionsDX11_EndUAVOverlap ? pagsDriverExtensionsDX11_EndUAVOverlap(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsGetVersionNumber() {
        return pagsGetVersionNumber ? pagsGetVersionNumber() : 0;
    }

    __declspec(dllexport) int WINAPI agsSetDisplayMode(void* param1, void* param2) {
        return pagsSetDisplayMode ? pagsSetDisplayMode(param1, param2) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_GetMaxClipRects(void* param) {
        return pagsDriverExtensionsDX11_GetMaxClipRects ? pagsDriverExtensionsDX11_GetMaxClipRects(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_IASetPrimitiveTopology(void* param) {
        return pagsDriverExtensionsDX11_IASetPrimitiveTopology ? pagsDriverExtensionsDX11_IASetPrimitiveTopology(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect(void* param) {
        return pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect ? pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirect(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect(void* param) {
        return pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect ? pagsDriverExtensionsDX11_MultiDrawIndexedInstancedIndirectCountIndirect(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_MultiDrawInstancedIndirect(void* param) {
        return pagsDriverExtensionsDX11_MultiDrawInstancedIndirect ? pagsDriverExtensionsDX11_MultiDrawInstancedIndirect(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect(void* param) {
        return pagsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect ? pagsDriverExtensionsDX11_MultiDrawInstancedIndirectCountIndirect(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_NotifyResourceBeginAllAccess(void* param) {
        return pagsDriverExtensionsDX11_NotifyResourceBeginAllAccess ? pagsDriverExtensionsDX11_NotifyResourceBeginAllAccess(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_NotifyResourceEndAllAccess(void* param) {
        return pagsDriverExtensionsDX11_NotifyResourceEndAllAccess ? pagsDriverExtensionsDX11_NotifyResourceEndAllAccess(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_NotifyResourceEndWrites(void* param) {
        return pagsDriverExtensionsDX11_NotifyResourceEndWrites ? pagsDriverExtensionsDX11_NotifyResourceEndWrites(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_NumPendingAsyncCompileJobs(void* param) {
        return pagsDriverExtensionsDX11_NumPendingAsyncCompileJobs ? pagsDriverExtensionsDX11_NumPendingAsyncCompileJobs(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_SetClipRects(void* param) {
        return pagsDriverExtensionsDX11_SetClipRects ? pagsDriverExtensionsDX11_SetClipRects(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_SetDepthBounds(void* param) {
        return pagsDriverExtensionsDX11_SetDepthBounds ? pagsDriverExtensionsDX11_SetDepthBounds(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_SetDiskShaderCacheEnabled(void* param) {
        return pagsDriverExtensionsDX11_SetDiskShaderCacheEnabled ? pagsDriverExtensionsDX11_SetDiskShaderCacheEnabled(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount(void* param) {
        return pagsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount ? pagsDriverExtensionsDX11_SetMaxAsyncCompileThreadCount(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_SetViewBroadcastMasks(void* param) {
        return pagsDriverExtensionsDX11_SetViewBroadcastMasks ? pagsDriverExtensionsDX11_SetViewBroadcastMasks(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX11_WriteBreadcrumb(void* param) {
        return pagsDriverExtensionsDX11_WriteBreadcrumb ? pagsDriverExtensionsDX11_WriteBreadcrumb(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_CreateDevice(void* param) {
        return pagsDriverExtensionsDX12_CreateDevice ? pagsDriverExtensionsDX12_CreateDevice(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_CreateFromDevice(void* param) {
        return pagsDriverExtensionsDX12_CreateFromDevice ? pagsDriverExtensionsDX12_CreateFromDevice(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_Destroy(void* param) {
        return pagsDriverExtensionsDX12_Destroy ? pagsDriverExtensionsDX12_Destroy(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_DestroyDevice(void* param) {
        return pagsDriverExtensionsDX12_DestroyDevice ? pagsDriverExtensionsDX12_DestroyDevice(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_PopMarker(void* param) {
        return pagsDriverExtensionsDX12_PopMarker ? pagsDriverExtensionsDX12_PopMarker(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_PushMarker(void* param) {
        return pagsDriverExtensionsDX12_PushMarker ? pagsDriverExtensionsDX12_PushMarker(param) : 0;
    }

    __declspec(dllexport) int WINAPI agsDriverExtensionsDX12_SetMarker(void* param) {
        return pagsDriverExtensionsDX12_SetMarker ? pagsDriverExtensionsDX12_SetMarker(param) : 0;
    }
}