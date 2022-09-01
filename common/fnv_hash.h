#pragma once
#include "../common/stdinc.h"
#include "common.h"

namespace fnv_hash
{
    template <typename T>
    std::enable_if_t<std::is_integral_v<T>, std::size_t>
    hash_seq(std::span<const T> seq, bool path_mode)
    {
        static constexpr uint64 fnv_prime = 16777619ull;

        auto real_seq = std::span(reinterpret_cast<const uchar *>(seq.data()),
                                  seq.size() * sizeof(T));

        std::size_t result = 2166136261u;

        for (auto element : real_seq)
        {
            if (path_mode)
            {
                //正斜杠+大写
                if (element == '\\')
                {
                    element = '/';
                }
                else if (element >= 'a' && element <= 'z')
                {
                    element -= 0x20;
                }
            }

            result ^= element;
            result = static_cast<uint>((result * fnv_prime) % 4294967296ull);
        }

        return result;
    }

    template <typename T>
    std::enable_if_t<std::is_integral_v<T>, std::span<const T>>
    get_string_span(const T *str)
    {
        std::size_t size = 0;

        if (str != nullptr)
        {
            auto c = str[size];

            while (c != 0)
            {
                ++size;
                c = str[size];
            }
        }

        return {str, size};
    }

    template <typename T>
    std::enable_if_t<std::is_integral_v<T>, std::vector<T>>
    get_string_vector(const T *str)
    {
        auto result = ranges::to_vector(get_string_span(str));
        result.push_back(0);

        return result;
    }

    //字符串哈希函数
    std::size_t hash_string(const std::string &str, bool path_mode);
    std::size_t hash_string(const char *str, bool path_mode);
    std::size_t hash_string(const uchar *str, bool path_mode);
    std::size_t hash_string(const GTAChar *str, bool path_mode);
} // namespace fnv_hash
