#pragma once
#include "../common/stdinc.h"
#include "../common/common.h"

class CStringTable
{
public:
    CStringTable() = default;
    ~CStringTable() = default;

    void LoadTable(const std::filesystem::path& filename);
    const GTAChar* GetString(const GTAChar* origin) const;

private:
    std::unordered_map<std::size_t, std::vector<GTAChar>> m_Table;
};
