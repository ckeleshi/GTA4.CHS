#include "string_table.h"
#include "../common/fnv_hash.h"

CStringTable& CStringTable::GetInstance()
{
    static CStringTable instance;

    return instance;
}

void CStringTable::LoadTable(const std::filesystem::path& filename)
{
    std::ifstream ifs(filename);

    if (!ifs)
    {
        return;
    }

    try
    {
        auto j = nlohmann::json::parse(ifs);

        for (auto it = j.begin(); it != j.end(); ++it)
        {
            auto origin = it.key();
            auto translated = it.value().get<std::string>();
            std::vector<GTAChar> utf16;

            if (translated.empty()) {
                continue;
            }

            try
            {
                utf8::utf8to16(translated.begin(), translated.end(), std::back_inserter(utf16));
                utf16.push_back(0);
                m_Table.emplace(fnv_hash::hash_string(origin.c_str(), false), std::move(utf16));
            }
            catch (utf8::exception&)
            {
                continue;
            }
        }
    }
    catch (nlohmann::json::exception&)
    {
        return;
    }
}

const GTAChar* CStringTable::GetString(const GTAChar* origin) const
{
    auto hash = fnv_hash::hash_string(origin, false);
    auto it = m_Table.find(hash);
    return it == m_Table.end() ? origin : it->second.data();
}
