#include "string_table.h"
#include "../common/fnv_hash.h"

void CStringTable::LoadTable(const std::filesystem::path& filename)
{
    auto logger = spdlog::get("gta4.chs");

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
            std::vector<GTAChar> worigin;
            std::vector<GTAChar> wtranslated;

            if (translated.empty()) {
                continue;
            }

            try
            {
                utf8::utf8to16(origin.begin(), origin.end(), std::back_inserter(worigin));
                worigin.push_back(0);
                utf8::utf8to16(translated.begin(), translated.end(), std::back_inserter(wtranslated));
                wtranslated.push_back(0);
                m_Table.emplace(fnv_hash::hash_string(worigin.data(), false), std::move(wtranslated));
            }
            catch (utf8::exception&)
            {
                logger->error("Utf8 convert failed.");
                continue;
            }
        }
    }
    catch (nlohmann::json::exception&)
    {
        logger->error("Json processing failed.");

        return;
    }
}

const GTAChar* CStringTable::GetString(const GTAChar* origin) const
{
    auto hash = fnv_hash::hash_string(origin, false);
    auto it = m_Table.find(hash);
    return it == m_Table.end() ? origin : it->second.data();
}
