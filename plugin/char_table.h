#pragma once
#include "../common/stdinc.h"
#include "../common/common.h"

class CCharTable
{
public:
    void LoadTable(const std::filesystem::path& filename);
    std::pair<uint, uint> GetCharPos(GTAChar chr) const;
    const std::vector<char32_t>& GetChars() const;

private:
    std::vector<char32_t> m_Chars;
};
