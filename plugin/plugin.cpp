#include "plugin.h"
#include "font.h"
#include "game.h"
#include "char_table.h"
#include "gta_string.h"
#include "gta_benchmark.h"
#include "gta_font.h"
#include "gta_game.h"
#include "gta_html.h"
#include "gta_menu.h"
#include "gta_phone.h"
#include "gta_save.h"
#include "gta_whm.h"
#include "gta_mail_reply.h"

CPlugin plugin;

void CPlugin::RegisterPatchSteps(batch_matching &batch_matcher)
{
    gta_benchmark::register_patches(batch_matcher);
    gta_font::register_patches(batch_matcher);
    gta_game::register_patches(batch_matcher);
    gta_html::register_patches(batch_matcher);
    gta_mail_reply::register_patches(batch_matcher);
    gta_menu::register_patches(batch_matcher);
    gta_phone::register_patches(batch_matcher);
    gta_save::register_patches(batch_matcher);
    gta_whm::register_patches(batch_matcher);
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

    batch_matching batch_matcher;

    RegisterPatchSteps(batch_matcher);

    if (batch_matcher.perform_search())
    {
        batch_matcher.write_log("gta4.chs");
    }

    if (!batch_matcher.is_all_succeed())
    {
        return false;
    }

    batch_matcher.run_callbacks();

    char_table.LoadTable(GetPluginAsset("char_table.dat"));
    whm_table.LoadTable(GetPluginAsset("whm_table.dat"));
    CFont::LoadTextures();

    return true;
}
