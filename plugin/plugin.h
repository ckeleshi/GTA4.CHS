#pragma once
#include "../common/stdinc.h"
#include "../common/common.h"
#include "batch_matching.h"
#include "char_table.h"
#include "whm_table.h"
#include "game.h"
#include "../common/module_path.h"
#include "gta_aslr.h"

class CPlugin
{
public:
    bool Init(HMODULE module);
    HANDLE GetGameModule() const;
    HANDLE GetPluginModule() const;
    std::filesystem::path GetGameRoot() const;
    std::filesystem::path GetRedirectRoot() const;
    std::filesystem::path GetPluginAsset(const char* rest_path) const; //为了支持中文字体文件名


    //保证进入构造函数之后这些对象已经初始化了
    ModulePath game_module_path, plugin_module_path;
    CCharTable char_table;
    CWhmTable whm_table;
    gta_aslr aslr_handler;
    CGame game;

private:
    static void RegisterPatchSteps(batch_matching& batch_matcher);
};

extern CPlugin plugin;
