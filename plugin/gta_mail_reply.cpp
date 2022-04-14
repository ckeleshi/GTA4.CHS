#include "gta_mail_reply.h"
#include "gta_string.h"
#include "plugin.h"

namespace gta_mail_reply
{
    //413332
    //5E49FF
    struct mr_read_eax_edx
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto ptr = reinterpret_cast<char*>(regs.eax + regs.edx);
            auto ptr2 = ptr;
            regs.ebx = utf8::unchecked::next(ptr2);
            auto offset = ptr2 - ptr;
            regs.eax = regs.edx + offset;
        }
    };

    //412A9C
    struct mr_read_eax_esi
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto ptr = reinterpret_cast<char*>(regs.eax + regs.esi);
            auto ptr2 = ptr;
            regs.edx = utf8::unchecked::next(ptr2);
            auto offset = ptr2 - ptr;
            regs.eax = regs.esi + offset;
        }
    };

    //413311
    struct mr_read_back_edi_eax
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto pclass = reinterpret_cast<class_for_mr*>(regs.edi);
            auto ptr = &pclass->f48[pclass->f448];
            auto ptr2 = ptr;
            regs.ebx = utf8::unchecked::prior(ptr2);
            pclass->f448 -= (ptr - ptr2);
        }
    };

    //5E49DE
    struct mr_read_back_esi_ecx
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto pclass = reinterpret_cast<class_for_mr*>(regs.esi);
            auto ptr = &pclass->f48[pclass->f448];
            auto ptr2 = ptr;
            regs.ebx = utf8::unchecked::prior(ptr2);
            pclass->f448 -= (ptr - ptr2);
        }
    };

    //412A7A
    struct mr_read_back_ecx_eax
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto pclass = reinterpret_cast<class_for_mr*>(regs.ecx);
            auto ptr = &pclass->f48[pclass->f448];
            auto ptr2 = ptr;
            regs.eax = utf8::unchecked::prior(ptr2);
            pclass->f448 -= (ptr - ptr2);
        }
    };

    //413523
    struct mr_write_edi_ecx
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto pclass = reinterpret_cast<class_for_mr*>(regs.edi);
            auto ptr = &pclass->f48[pclass->f448];
            auto offset = utf8::unchecked::append(regs.ebx, ptr) - ptr;
            pclass->f448 += offset;
        }
    };

    //5E4CCE
    struct mr_write_esi_eax
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto pclass = reinterpret_cast<class_for_mr*>(regs.esi);
            auto ptr = &pclass->f48[pclass->f448];
            auto offset = utf8::unchecked::append(regs.ebx, ptr) - ptr;
            pclass->f448 += offset;
        }
    };

    //5E5205
    struct mr_write_ebp_esi
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto ptr = reinterpret_cast<char*>(regs.esi + regs.ebp);

            //对&nbsp之类字符的特殊处理，从sub_5E5250的逻辑来看，这么偷懒是可以的
            //目前中文字库最小的值是0x2014
            if (regs.ecx > 0x100 && regs.ecx < 0x200)
                regs.ecx -= 0x100;

            auto offset = utf8::unchecked::append(regs.ecx, ptr) - ptr;
            regs.esi += offset;
            regs.ecx = regs.edi;
        }
    };

    //8C5AB1
    struct dlc_truncate
    {
        void operator()(injector::reg_pack& regs) const
        {
            auto ptr = &plugin.game.game_addr.pDLCTruncateBuffer[regs.eax + regs.edx];

            auto offset = utf8::unchecked::append(*reinterpret_cast<const GTAChar*>(regs.esi), ptr) - ptr;
            regs.edx += offset - 1;
        }
    };

    void register_patches(batch_matching& batch_matcher)
    {
        //Native: ADD_FIRST_N_CHARACTERS_OF_STRING_TO_HTML_SCRIPT_OBJECT(75FC34EF)里面
        //邮件回复
        batch_matcher.register_step("E8 ? ? ? ? 8D 44 24 08 50 56 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                //要用的函数的地址
                plugin.game.game_addr.fnMailAppendByteString = injector::GetBranchDestination(addresses[0].i()).get();

                //接收宽字符，但添加的是utf8
                injector::MakeCALL(addresses[0].i(11), gta_string::gtaMailAppendWideStringAsUtf8);

                //去掉span标签
                injector::MakeNOP(addresses[0].i(-6), 11);
                injector::MakeNOP(addresses[0].i(16), 11);
            });

        batch_matcher.register_step("0F B6 1C 10 8D 42 01", 6, [](const byte_pattern::result_type& addresses)
            {
                //读utf8序列
                //用到"!--"的函数
                injector::MakeInline<mr_read_eax_edx>(addresses[0].i(), addresses[0].i(7));

                //IDA Names: const CHtmlParser::`vftable'
                //虚表的第三个函数
                injector::MakeInline<mr_read_eax_edx>(addresses[4].i(), addresses[4].i(7));
            });

        //CHtmlParser虚表第三个函数开头调用了MailGetStringChar
        //在MailGetStringChar里面
        batch_matcher.register_step("0F B6 14 30 8D 46 01", 7, [](const byte_pattern::result_type& addresses)
            {
                //读utf8序列
                injector::MakeInline<mr_read_eax_esi>(addresses[0].i(), addresses[0].i(7));
            });

        //在上述用到"!--"的函数里面
        batch_matcher.register_step("FF 8F 48 04 00 00 8B 87 48 04 00 00 0F B6 5C 38 48", 1, [](const byte_pattern::result_type& addresses)
            {
                //读上一个utf8序列
                injector::MakeInline<mr_read_back_edi_eax>(addresses[0].i(), addresses[0].i(17));
            });

        //在上述用到"!--"的函数里面
        batch_matcher.register_step("FF 8E 48 04 00 00 8B 8E 48 04 00 00 0F B6 5C 31 48", 1, [](const byte_pattern::result_type& addresses)
            {
                //读上一个utf8序列
                injector::MakeInline<mr_read_back_esi_ecx>(addresses[0].i(), addresses[0].i(17));
            });

        //在上述CHtmlParser虚表第三个函数里面
        batch_matcher.register_step("FF 89 48 04 00 00 8B 81 48 04 00 00 0F B6 44 08 48", 1, [](const byte_pattern::result_type& addresses)
            {
                //读上一个utf8序列
                injector::MakeInline<mr_read_back_ecx_eax>(addresses[0].i(), addresses[0].i(17));
            });

        //在上述用到"!--"的函数里面
        batch_matcher.register_step("8D 14 39 41 89 8F 48 04 00 00 88 5A 48", 1, [](const byte_pattern::result_type& addresses)
            {
                //写utf8序列
                injector::MakeInline<mr_write_edi_ecx>(addresses[0].i(), addresses[0].i(13));
            });

        //在上述CHtmlParser虚表第三个函数里面
        batch_matcher.register_step("8B 86 48 04 00 00 8D 0C 06 40 89 86 48 04 00 00 88 59 48", 1, [](const byte_pattern::result_type& addresses)
            {
                //写utf8序列
                injector::MakeInline<mr_write_esi_eax>(addresses[0].i(), addresses[0].i(19));
            });

        //在上述CHtmlParser虚表第三个函数调用的函数里面
        batch_matcher.register_step("88 0C 2E 8B CF", 1, [](const byte_pattern::result_type& addresses)
            {
                //写utf8序列
                injector::MakeInline<mr_write_ebp_esi>(addresses[0].i(), addresses[0].i(6));
            });

        //Native: CONVERT_THEN_ADD_STRING_TO_HTML_SCRIPT_OBJECT(72EC0AA6)的转换过程里面
        batch_matcher.register_step("C6 84 10 ? ? ? ? 3F", 1, [](const byte_pattern::result_type& addresses)
            {
                plugin.game.game_addr.pDLCTruncateBuffer = *addresses[0].p<char*>(3);

                //DLC不支持日语而加的转换逻辑
                injector::MakeInline<dlc_truncate>(addresses[0].i(), addresses[0].i(8));
            });

        //Native: GET_FIRST_N_CHARACTERS_OF_STRING(108B4A25)调用strncpy的地方
        batch_matcher.register_step("56 50 68 ? ? ? ? E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(7), gta_string::gtaUTF8strncpy);
                injector::MakeNOP(addresses[0].i(24), 7);
            });

        //batch_matcher.register_step("", 1, [](const byte_pattern::result_type& addresses)
//    {
//
//    });
    }
}
