#pragma once
#include "stdinc.h"

class ModulePath
{
public:
    typedef std::filesystem::path path_type;

    ModulePath() = default;
    explicit ModulePath(HMODULE module);

    void SetModule(HMODULE module);
    HMODULE GetModule() const;
    path_type GetModuleDir() const;
    path_type GetRelativeSub(const path_type& rest) const;

private:
    HMODULE m_Module;
    path_type m_ModuleDir;
};
