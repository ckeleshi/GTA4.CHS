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

void CPlugin::RegisterPatchSteps(batch_matching& batch_matcher)
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

std::filesystem::path CPlugin::GetRedirectRoot() const
{
    return plugin_module_path.GetModuleDir() / "GTA4.CHS/redirect";
}

std::filesystem::path CPlugin::GetPluginAsset(const char* rest_path) const
{
    return plugin_module_path.GetModuleDir() / "GTA4.CHS" / rest_path;
}

bool CPlugin::Init(HMODULE module)
{
    game_module_path.SetModule(::GetModuleHandleW(nullptr));
    plugin_module_path.SetModule(module);
    aslr_handler.SetGameBase(game_module_path.GetModule());

    batch_matching batch_matcher;

    batch_matcher.set_aslr(aslr_handler);

    //必须先注册再读缓存
    RegisterPatchSteps(batch_matcher);

    batch_matcher.load_cache(GetPluginAsset("pattern_cache.dat"));

    //验证缓存的操作放在byte_pattern中
    if (batch_matcher.perform_search())
    {
        //如果有进行过搜索则写入，不论成功还是失败
        batch_matcher.write_cache(GetPluginAsset("pattern_cache.dat"));
        batch_matcher.write_log(GetPluginAsset("pattern_log.log"), false);
    }

    if (!batch_matcher.is_all_succeed())
    {
        return false;
    }

    batch_matcher.run_callbacks();

    char_table.LoadTable(GetPluginAsset("char_table.dat"));
    whm_table.LoadTable(GetPluginAsset("whm_table.dat"));

    return true;
}
