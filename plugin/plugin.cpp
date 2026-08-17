#include "plugin.h"
#include "char_table.h"
#include "font.h"
#include "game.h"
#include "gta_benchmark.h"
#include "gta_font.h"
#include "gta_game.h"
#include "gta_html.h"
#include "gta_mail_reply.h"
#include "gta_menu.h"
#include "gta_phone.h"
#include "gta_save.h"
#include "gta_string.h"
#include "gta_whm.h"

CPlugin plugin;

void CPlugin::RegisterPatchSteps()
{
    gta_benchmark::register_patches();
    gta_font::register_patches();
    gta_game::register_patches();
    gta_html::register_patches();
    gta_mail_reply::register_patches();
    gta_menu::register_patches();
    gta_phone::register_patches();
    gta_save::register_patches();
    gta_whm::register_patches();
}

HANDLE CPlugin::GetGameModule() const
{
    return game_module_path.GetModule();
}

HANDLE CPlugin::GetPluginModule() const
{
    return plugin_module_path.GetModule();
}

std::filesystem::path CPlugin::GetGameRoot() const
{
    return game_module_path.GetModuleDir();
}

std::filesystem::path CPlugin::GetPluginAsset(const std::filesystem::path &rest_path) const
{
    return plugin_module_path.GetModuleDir() / "GTA4.CHS" / rest_path;
}

bool CPlugin::Init(HMODULE module)
{
    game_module_path.SetModule(::GetModuleHandleW(nullptr));
    plugin_module_path.SetModule(module);

    RegisterPatchSteps();

    char_table.LoadTable(GetPluginAsset("char_table.dat"));
    whm_table.LoadTable(GetPluginAsset("whm_table.dat"));
    string_table.LoadTable(GetPluginAsset("custom_translations.json"));

    return true;
}
