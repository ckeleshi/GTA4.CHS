#include "plugin.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        return plugin.Init(module) ? TRUE : FALSE;
    }

    return TRUE;
}
