#include "gta_aslr.h"

void gta_aslr::SetGameBase(std::intptr_t base)
{
    m_Base = base;
}

void gta_aslr::SetGameBase(HMODULE module)
{
    m_Base = reinterpret_cast<std::intptr_t>(module);
}

void gta_aslr::SetIdaBase(std::intptr_t base)
{
    m_IdaBase = base;
}

std::intptr_t gta_aslr::GetGameAddr(std::intptr_t ida_addr) const
{
    return ida_addr + m_Base - m_IdaBase;
}

std::intptr_t gta_aslr::GetIdaAddr(std::intptr_t game_addr) const
{
    return game_addr - m_Base + m_IdaBase;
}
