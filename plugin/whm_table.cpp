#include "whm_table.h"
#include "../common/fnv_hash.h"

void CWhmTable::LoadTable(const std::filesystem::path& filename)
{
    m_offsets.clear();
    m_strings.clear();

    BinaryFile file(filename, "rb");

    if (!file)
        return;

    std::vector<WhmTextData> entries;
    std::vector<uchar> strings;

    file.ReadArray2(entries);
    file.ReadArray2(strings);

    for (auto& entry : entries)
    {
        m_offsets.emplace(entry.hash, entry.offset); //重复的hash添加不进去
    }

    m_strings.swap(strings);
}

const uchar* CWhmTable::GetTranslated(const uchar* src) const
{
    auto it = m_offsets.find(fnv_hash::hash_string(src, false));

    if (it == m_offsets.end())
    {
        return src;
    }
    else
    {
        return &m_strings[it->second];
    }
}
