#include <Windows.h>
#include <iostream>

// gunakan injector dari https://github.com/guidedhacking/GuidedHacking-Injector
//============================================================================================================

DWORD WINAPI InjectDllFunc(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "[*] F1 for HP \n[*] F2 for Ki\n[*] F3 for Stop" << std::endl;

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"nioh.exe");

    bool bHealth = false, bKi = false;
    int counthp = 1;
    int countki = 1;
    ULONG_PTR hpOffsets = { 0x0020 };
    ULONG_PTR kiOffsets = { 0x0040 };

    uintptr_t* localPlayer = (uintptr_t*)(moduleBase + 0x018A0490);
    uintptr_t* entity = (uintptr_t*)(*localPlayer + 0x0240);

    uint64_t* hpAddress = (uint64_t*)(*entity + 0x0020);
    float* kiAddress = (float*)(*entity + 0x0040);

 
    while (1)
    {
        if (GetAsyncKeyState(VK_F3) & 1) {
            break;
        }

        if (GetAsyncKeyState(VK_F1) & 1) {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_F2) & 1) {
            bKi = !bKi;
        }


        if (localPlayer)
        {
            if (bHealth) {
                
                uint64_t unlimitedHealth = (uint64_t)9999;
                *hpAddress = unlimitedHealth;
            }

            if (bKi) {
                float unlimitedKi = 9999.f;
                *kiAddress = unlimitedKi;
                
            }
        }
        Sleep(5);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InjectDllFunc, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

