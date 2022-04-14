#include "gta_string.h"
#include "plugin.h"
#include "../common/fnv_hash.h"

namespace gta_string
{
    static std::unordered_map<std::size_t, std::vector<GTAChar>> truncated_text_map; //key是单字节字符串的hash, value是原始宽字符串的内容，包含结尾0

    constexpr uchar pad_byte = 0xA7u; //用于填充的垃圾值

    unsigned gtaWcslen(const GTAChar* str)
    {
        auto str2 = str;
        unsigned len = 0;

        while (*str2 != 0)
        {
            ++str2;
            ++len;
        }

        return len;
    }

    //8C5510
    uchar* gtaTruncateString(uchar* dst, const GTAChar* src, unsigned size)
    {
        std::size_t copied_size = 0;

        if (src != nullptr)
        {
            if (size > 1)
            {
                while (copied_size < size - 1)
                {
                    auto c = src[copied_size];

                    if (c == 0)
                        break;

                    //如果低字节恰好是0，就随便用一个整数填充，防止结果中出现意外的0，最好是非ASCII
                    //游戏也只是用结果进行字符串比较，应该没有问题
                    if ((c & 0xFFu) == 0)
                        c |= pad_byte;

                    dst[copied_size] = static_cast<uchar>(c);
                    ++copied_size;
                }

                //将dst的hash和src的数据存入map，供gtaExpandString查找
                //注意要以实际复制的长度取dst
                truncated_text_map.emplace(fnv_hash::hash_seq(std::span<const uchar>(dst, copied_size), false), fnv_hash::get_string_vector(src));
            }
        }

        dst[copied_size] = 0;

        return dst;
    }

    //909F50
    void gtaExpandString(const uchar* src, GTAChar* dst)
    {
        if (src == nullptr || dst == nullptr)
            return;

        auto src_span = fnv_hash::get_string_span(src);

        auto wide_string_it = truncated_text_map.find(fnv_hash::hash_seq(src_span, false));

        if (wide_string_it != truncated_text_map.end())
        {
            ranges::copy(wide_string_it->second, dst);
        }
        else
        {
            *ranges::copy(src_span, dst).out = 0;
        }
    }

    void gtaExpandString2(GTAChar* dst, const uchar* src, int a8)
    {
        gtaExpandString(src, dst);
    }

    //8FAC40
    void gtaExpandString3(GTAChar* dst, const uchar* src, int a8)
    {
        auto src_span = fnv_hash::get_string_span(src);
        auto src_it = src_span.begin();
        auto src_end_it = src_span.end();

        while (!utf8::is_valid(src_it, src_end_it))
            ++src_it;

        gtaExpandStringGxt(&*src_it, dst);
    }

    //91EBC0
    void gtaTruncateString2(const GTAChar* src, uchar* dst)
    {
        auto src_span = fnv_hash::get_string_span(src);
        *utf8::utf16to8(src_span.begin(), src_span.end(), dst) = 0;
    }

    //909F50
    void gtaExpandStringGxt(const uchar* src, GTAChar* dst)
    {
        auto src_span = fnv_hash::get_string_span(src);

        auto src_it = src_span.begin();
        auto src_end_it = src_span.end();

        if (utf8::is_valid(src_it, src_end_it))
        {
            *utf8::utf8to16(src_span.begin(), src_end_it, dst) = 0;
        }
        else
        {
            //遇到无效的utf8序列(如网页中未翻译的FIGS字符)，则直接扩展成utf16
            *ranges::copy(src_span, dst).out = 0;
        }
    }

    //replace call at 5E7AD8
    void __stdcall gtaMailAppendWideStringAsUtf8(int id, const GTAChar* str)
    {
        std::vector<uchar> u8_buffer;
        u8_buffer.reserve(2048);

        auto src_span = fnv_hash::get_string_span(str);

        utf8::utf16to8(src_span.begin(), src_span.end(), std::back_inserter(u8_buffer));
        u8_buffer.push_back(0);

        plugin.game.MailAppendByteString(id, u8_buffer.data());
    }

    uchar* gtaUTF8strncpy(uchar* dest, const uchar* source, unsigned size)
    {
        auto dest_ptr = dest;

        for (unsigned index = 0; index < size; ++index)
        {
            auto cp = utf8::unchecked::next(source);

            if (cp == 0)
                break;

            dest_ptr = utf8::unchecked::append(cp, dest_ptr);
        }

        *dest_ptr = 0;
        return dest;
    }

    void* gtaSpecialMemmove(uchar* dest, const uchar* source, unsigned size)
    {
        std::strcpy(reinterpret_cast<char*>(dest), reinterpret_cast<const char*>(source));
        return dest;
    }

    unsigned gtaUTF8Strlen(const uchar* str)
    {
        if (str == nullptr)
            return 0;

        unsigned len = 0;

        while (*str != 0)
        {
            utf8::unchecked::next(str);
            ++len;
        }

        return len;
    }
}
