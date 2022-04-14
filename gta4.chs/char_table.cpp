#include "char_table.h"

void CCharTable::LoadTable(const std::filesystem::path& filename)
{
    BinaryFile file(filename, "rb");
    file.ReadArray2(m_Chars);
}

std::pair<uint, uint> CCharTable::GetCharPos(GTAChar chr) const
{
    auto char_it = ranges::lower_bound(m_Chars, chr);

    if (char_it != m_Chars.end() && *char_it == chr)
    {
        auto diff = char_it - m_Chars.begin();

        return { diff / g_CharsPerLine,diff % g_CharsPerLine };
    }
    else
    {
        return GetCharPos(L'？');
    }
}

const std::vector<char32_t>& CCharTable::GetChars() const
{
    return m_Chars;
}
