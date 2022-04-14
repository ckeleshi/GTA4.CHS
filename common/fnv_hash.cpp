#include "fnv_hash.h"

namespace fnv_hash
{
    std::span<const uchar> get_string_span(const std::string& str)
    {
        return { reinterpret_cast<const uchar*>(str.c_str()), str.size() };
    }

    std::size_t hash_string(const std::string& str, bool path_mode)
    {
        return hash_seq(get_string_span(str), path_mode);
    }

    std::size_t hash_string(const char* str, bool path_mode)
    {
        return hash_seq(get_string_span(str), path_mode);
    }

    std::size_t hash_string(const uchar* str, bool path_mode)
    {
        return hash_seq(get_string_span(str), path_mode);
    }

    std::size_t hash_string(const GTAChar* str, bool path_mode)
    {
        return hash_seq(get_string_span(str), path_mode);
    }
}
