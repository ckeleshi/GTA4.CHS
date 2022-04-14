#include "batch_matching.h"
#include "gta_aslr.h"

void batch_matching::set_aslr(const gta_aslr& handler)
{
    aslr_handler = handler;
}

void batch_matching::register_step(const char* pattern, std::size_t expected_size, callback_type callback, bool confirm)
{
    match_step step;
    step.confirm = confirm;
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
    bool result = false;

    auto start_time = std::chrono::steady_clock::now();

    byte_pattern pattern_obj;

    for (auto& step : _steps)
    {
        //写入缓存的数据，验证不成功时才进行搜索
        pattern_obj.set_pattern(step.first.c_str());
        pattern_obj.set_results(step.second.result);

        //如果缓存结果个数跟要求个数不一样，也要重新搜索
        if (step.second.expected_size != step.second.result.size() || !pattern_obj.validate_result())
        {
            result = true;
            pattern_obj.search();
            step.second.result = pattern_obj.get();
        }
    }

    auto end_time = std::chrono::steady_clock::now();
    _last_cost_ms = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count()) / 1000.0;

    return result;
}

bool batch_matching::is_all_succeed() const
{
    return ranges::all_of(_steps,
        [](const std::pair<std::string, match_step>& step)
        {
            return step.second.expected_size == step.second.result.size();
        });
}

void batch_matching::run_callbacks() const
{
    for (auto& step : _steps)
    {
        if (step.second.confirm)
        {
            step.second.callback(step.second.result);
        }
    }
}

void batch_matching::write_log(const std::filesystem::path& filename, bool invalid_only) const
{
    std::FILE* out = std::fopen(filename.string().c_str(), "wb");

    if (!out)
    {
        return;
    }

    std::fprintf(out, "Time Cost: %.2lf ms\n\n", _last_cost_ms);

    for (auto& step : _steps)
    {
        if (invalid_only && step.second.expected_size != step.second.result.size())
        {
            continue;
        }

        std::fprintf(out, "Pattern: %s\n", step.first.c_str());
        std::fprintf(out, "Expected: %u\n", step.second.expected_size);
        std::fprintf(out, "Found: %u\n", step.second.result.size());

        for (std::size_t index = 0; index < step.second.result.size(); ++index)
        {
            std::fprintf(out, "Address%u: 0x%08X\n", index + 1, aslr_handler.GetIdaAddr(step.second.result[index].i()));
        }

        std::fprintf(out, "\n");
    }

    std::fflush(out);
    std::fclose(out);
}

void batch_matching::write_cache(const std::filesystem::path& filename) const
{
    BinaryFile out(filename, "wb");

    if (!out)
    {
        return;
    }

    uint step_count = _steps.size();
    out.Write(step_count);

    for (auto& step : _steps)
    {
        //写入pattern
        out.WriteArray2(std::span{ step.first });

        std::vector<std::intptr_t> transformed_addr(step.second.result.size());

        //转换成ida地址
        ranges::transform(step.second.result, transformed_addr.begin(), [this](const memory_pointer& p)
            {
                return aslr_handler.GetIdaAddr(p.i());
            });

        //写入结果
        out.WriteArray2(std::span{ transformed_addr });
    }

    out.Close();
}

void batch_matching::load_cache(const std::filesystem::path& filename)
{
    BinaryFile in(filename, "rb");

    if (!in)
    {
        return;
    }

    uint step_count;
    in.Read(step_count);

    for (uint step_index = 0; step_index < step_count; ++step_index)
    {
        std::string pattern_str;
        std::vector<char> pattern_bytes;
        in.ReadArray2(pattern_bytes);
        pattern_str.assign(pattern_bytes.begin(), pattern_bytes.end());

        std::vector<std::intptr_t> transformed_addr;
        in.ReadArray2(transformed_addr);

        auto step_it = _steps.find(pattern_str);

        //忽略没注册的pattern
        if (step_it == _steps.end())
        {
            continue;
        }

        //填入储存的结果
        step_it->second.result.resize(transformed_addr.size());
        ranges::transform(transformed_addr, step_it->second.result.begin(), [this](std::intptr_t i)
            {
                return memory_pointer(aslr_handler.GetIdaAddr(i));
            });
    }
}
