#pragma once
#include "../common/stdinc.h"

class CWhmTable
{
public:
    void LoadTable(const std::filesystem::path& filename);

    //查表，将whm的原文替换成译文
    const uchar* GetTranslated(const uchar* src) const;

private:
    std::unordered_map<std::size_t, std::uintptr_t> m_offsets;
    std::vector<uchar> m_strings;
};
