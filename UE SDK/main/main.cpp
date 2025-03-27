#include "framework.h"

DWORD WINAPI Main(LPVOID lpParam)
{
    AllocConsole();
    FILE* File = nullptr;
    freopen_s(&File, "CONOUT$", "w+", stdout);
    Sleep(5000);
    if (!SDK::Init())
    {
        std::cout << "Failed to initalize core aspects of the SDK" << std::endl;
        return 1;
    }

    std::cout << "Engine_Version: " << SDK::UE::GetEngineVersion() << std::endl;
    std::cout << "Fortnite_Version: " << SDK::UE::GetFortniteVersion() << std::endl;
    std::cout << "Fortnite_CL: " << SDK::UE::GetFortniteCL() << std::endl;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == 1)
        CreateThread(0, 0, Main, lpReserved, 0, 0);
    return TRUE;
}

