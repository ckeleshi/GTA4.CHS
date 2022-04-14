#include "module_path.h"

ModulePath::ModulePath(HMODULE module)
{
    m_Module = module;
    SetModule(module);
}

void ModulePath::SetModule(HMODULE module)
{
    m_ModuleDir.clear();

    m_Module = module;

    wchar_t w_path[512];
    ::GetModuleFileNameW(module, w_path, 512);
    path_type module_path = w_path;
    m_ModuleDir = module_path.parent_path();
}

HMODULE ModulePath::GetModule() const
{
    return m_Module;
}

ModulePath::path_type ModulePath::GetModuleDir() const
{
    return m_ModuleDir;
}

ModulePath::path_type ModulePath::GetRelativeSub(const path_type& rest) const
{
    if (m_ModuleDir.empty())
    {
        return {};
    }

    return m_ModuleDir / rest;
}
