#pragma once
#include "../common/stdinc.h"
#include "../common/common.h"

class CStringTable
{
public:
    static CStringTable& GetInstance();

    void LoadTable(const std::filesystem::path& filename);
    const GTAChar* GetString(const GTAChar* origin) const;

private:
    CStringTable() = default;
    ~CStringTable() = default;

    std::unordered_map<std::uint64_t, std::vector<GTAChar>> m_Table;
};
