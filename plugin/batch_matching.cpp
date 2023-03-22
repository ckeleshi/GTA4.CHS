#include "batch_matching.h"

void batch_matching::register_step(const char *pattern, std::size_t expected_size, callback_type callback,
                                   bool run_callback)
{
    match_step step;
    step.run_callback = run_callback;
    step.expected_size = expected_size;
    step.callback = std::move(callback);

    _steps.emplace(pattern, std::move(step));
}

void batch_matching::clear()
{
    _steps.clear();
}

bool batch_matching::perform_search()
{
    auto start_time = std::chrono::steady_clock::now();

    byte_pattern pattern_obj;

    for (auto &step : _steps)
    {
        // 写入缓存的数据，验证不成功时才进行搜索
        pattern_obj.set_pattern(step.first.c_str());

        pattern_obj.search();
        step.second.result = pattern_obj.get();
    }

    auto end_time = std::chrono::steady_clock::now();
    _last_cost_ms =
        static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count()) /
        1000.0;

    return true;
}

bool batch_matching::is_all_succeed() const
{
    return ranges::all_of(_steps, [](const std::pair<std::string, match_step> &step) {
        return step.second.expected_size == step.second.result.size();
    });
}

void batch_matching::run_callbacks() const
{
    for (auto &step : _steps)
    {
        if (step.second.run_callback)
        {
            step.second.callback(step.second.result);
        }
    }
}

void batch_matching::write_log(const std::filesystem::path &filename) const
{
#ifdef _DEBUG
    auto hExe = GetModuleHandleW(NULL);
    auto hExeInt = reinterpret_cast<std::intptr_t>(hExe);

    std::FILE *out = std::fopen(filename.string().c_str(), "wt");

    if (!out)
    {
        return;
    }

    fmt::fprintf(out, "Time Cost: %.2lf ms\n\n", _last_cost_ms);

    for (auto &step : _steps)
    {
        fmt::fprintf(out, "Pattern: %s\n", step.first.c_str());
        fmt::fprintf(out, "Expected: %u\n", step.second.expected_size);
        fmt::fprintf(out, "Found: %u\n", step.second.result.size());

        for (std::size_t index = 0; index < step.second.result.size(); ++index)
        {
            fmt::fprintf(out, "Address%u: 0x%08X\n", index + 1, step.second.result[index].i() - hExeInt + 0x400000);
        }

        fmt::fprintf(out, "\n");
    }

    std::fflush(out);
    std::fclose(out);
#endif
}
