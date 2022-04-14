#pragma once
#include "../common/stdinc.h"
#include "byte_pattern.h"
#include "gta_aslr.h"

class batch_matching
{
public:
    typedef std::function<void(const byte_pattern::result_type&)> callback_type;

    struct match_step
    {
        bool confirm;
        std::size_t expected_size;
        callback_type callback;
        byte_pattern::result_type result;
    };

    void set_aslr(const gta_aslr& handler);
    void register_step(const char* pattern, std::size_t expected_size, callback_type callback, bool confirm = true);
    void clear();
    bool perform_search(); //返回是否进行过搜索
    bool is_all_succeed() const;
    void run_callbacks() const;

    void write_log(const std::filesystem::path& filename, bool invalid_only) const;
    void write_cache(const std::filesystem::path& filename) const;
    void load_cache(const std::filesystem::path& filename);

private:
    std::unordered_map<std::string, match_step> _steps;
    double _last_cost_ms;
    gta_aslr aslr_handler;
};
