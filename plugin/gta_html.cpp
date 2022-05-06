#include "gta_html.h"
#include "gta_string.h"

namespace gta_html
{
void register_patches(batch_matching &batch_matcher)
{
    //替换html字符串处理过程

    // Native: ADD_STRING_TO_HTML_SCRIPT_OBJECT(7EB70379)里面
    batch_matcher.register_step("8B F8 8D 44 24 08 50 57 E8", 1, [](const byte_pattern::result_type &addresses) {
        injector::MakeCALL(addresses[0].i(8), gta_string::gtaTruncateString2);
    });

    // IDA Names: const CHtmlDataNode::`vftable'
    //两个使用此虚表的函数
    batch_matcher.register_step("74 0A 50 55 E8", 2, [](const byte_pattern::result_type &addresses) {
        injector::MakeCALL(addresses[0].i(4), gta_string::gtaExpandStringGxt);
        injector::MakeCALL(addresses[1].i(4), gta_string::gtaExpandStringGxt);
    });

    // Native: GET_STRING_FROM_STRING(434534BE)，调用strncpy的地方
    batch_matcher.register_step("7F 4B 56 52 8D 44 24 14 50 E8", 1, [](const byte_pattern::result_type &addresses) {
        injector::MakeCALL(addresses[0].i(9), gta_string::gtaUTF8strncpy);
        injector::MakeNOP(addresses[0].i(27), 5);
    });

    //跟上面同一个函数，调用memmove的地方
    batch_matcher.register_step("03 C3 57 50 68 ? ? ? ? E8", 1, [](const byte_pattern::result_type &addresses) {
        injector::MakeCALL(addresses[0].i(9), gta_string::gtaSpecialMemmove);
        injector::MakeNOP(addresses[0].i(25), 7);
    });

    //在调用CFont_GetMaxWordWidth之前调用的函数里面，设置相关CFont参数的
    //网页邮件文字的比例值，直接用日语版的数值
    batch_matcher.register_step("80 3D ? ? ? ? 6A F3 0F 10 05", 1, [](const byte_pattern::result_type &addresses) {
        // 5E2069
        injector::WriteMemory<uchar>(addresses[0].i(24), 0xEBu, true);
    });

    // Native: GET_WEB_PAGE_LINK_AT_POSN(C1C5B1B)的判断逻辑，跟30.0比较
    batch_matcher.register_step("F3 0F 10 5A 24 72 5C", 1, [](const byte_pattern::result_type &addresses) {
#if 1
        // 5D6CE0
        injector::WriteMemory<uchar>(addresses[0].i(5), 0xEBu, true);
#else
                static const float min_link_width = 5.0f;

                injector::WriteMemory(addresses[0].i(0xCD6 - 0xCF4), &min_link_width, true);
                injector::WriteMemory(addresses[0].i(0xD53 - 0xCF4), &min_link_width, true);
#endif
    });

    // IDA Names: const CHtmlTextFormat::`vftable'
    //其中一个使用此虚表的函数
    //计算超链接响应宽度的逻辑
    batch_matcher.register_step("F3 0F 5C 4B 04 0F 2E C1 9F F6 C4 44 7B 27", 1,
                                [](const byte_pattern::result_type &addresses) {
                                    // 5D9B43
                                    // injector::WriteMemory<uchar>(addresses[0].i(12), 0xEBu, true);
                                });

    // batch_matcher.register_step("", 1, [](const byte_pattern::result_type& addresses)
    //     {
    //
    //     });
}
} // namespace gta_html
