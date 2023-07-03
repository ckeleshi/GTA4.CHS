#include "plugin.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        spdlog::basic_logger_mt("gta4.chs", plugin.GetPluginAsset("gta4.chs.log").string(), true);
        return plugin.Init(module) ? TRUE : FALSE;
    }

    return TRUE;
}
