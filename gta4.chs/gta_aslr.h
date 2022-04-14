#pragma once
#include "../common/stdinc.h"

class gta_aslr
{
public:
    void SetGameBase(std::intptr_t base);
    void SetGameBase(HMODULE module);
    void SetIdaBase(std::intptr_t base);

    std::intptr_t GetGameAddr(std::intptr_t ida_addr) const;
    std::intptr_t GetIdaAddr(std::intptr_t game_addr) const;

private:
    std::intptr_t m_Base = 0x400000;
    std::intptr_t m_IdaBase = 0x400000;
};
