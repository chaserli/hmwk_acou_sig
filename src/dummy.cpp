#include <iostream>

#ifdef __linux__
void __attribute__((constructor)) load_so()
{
    std::cout << "Dll attached\n";
}

void __attribute__((destructor)) unload_so()
{
    std::cout << "Dll detached\n";
}
#endif

#ifdef _WIN32
#include <Windows.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        std::cout << "Dll process attached\n";
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        if (lpvReserved != nullptr)
        {
            std::cout << "Dll process detached\n";
            break;
        }
        break;
    }
    return TRUE;
}
#endif
