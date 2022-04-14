#include "gta_font.h"
#include "font.h"
#include "plugin.h"
#include "gta_aslr.h"

namespace gta_font
{
    static const char* __stdcall GetTextFileName(int)
    {
        return "chinese.gxt";
    }

    //922095
    __declspec(naked) void ProcessToken_Epilog()
    {
        __asm
        {
            pop edi;

            call plugin.game.game_addr.fnFont_ProcessToken;
            or eax, 0x80000000;

            push edi;
            ret;
        }
    }

    //922120
    __declspec(naked) void ProcessToken_Epilog2()
    {
        __asm
        {
            and edi, 0x7FFFFFFF;
            movzx eax, word ptr[edi - 4];
            sub edi, 4;
            ret;
        }
    }

    //922147
    __declspec(naked) void ProcessToken_Epilog3()
    {
        __asm
        {
            and edi, 0x7FFFFFFF;
            mov[esp + 0x16], cl; //原代码是[esp+0x12]，因为栈里多了一个返回地址所以是+0x16
            test cl, cl;
            ret;
        }
    }

    void register_patches(batch_matching& batch_matcher)
    {
        //搜索"~%c~"找到CFont::ProcessString

         //ProcessToken epilog
        batch_matcher.register_step("89 AC 24 80 00 00 00 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                plugin.game.game_addr.fnFont_ProcessToken = injector::GetBranchDestination(addresses[0].i(7)).get();
                injector::MakeCALL(addresses[0].i(7), ProcessToken_Epilog);
            });

        //ProcessString token branch2
        batch_matcher.register_step("0F B7 47 FC 83 EF 04", 1, [](const std::vector<memory_pointer>& addresses)
            {
                injector::MakeCALL(addresses[0].i(), ProcessToken_Epilog2);
                injector::MakeNOP(addresses[0].i(5), 2);
            });

        //ProcessString token branch3
        batch_matcher.register_step("88 4C 24 12 84 C9", 1, [](const std::vector<memory_pointer>& addresses)
            {
                injector::MakeCALL(addresses[0].i(), ProcessToken_Epilog3);
                injector::MakeNOP(addresses[0].i(5), 1);
            });

        //CFont::ProcessString使用了
        //跳过单词
        batch_matcher.register_step("57 8B 7C 24 08 85 FF 75 04 33 C0 5F C3 56", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeJMP(addresses[0].i(), CFont::SkipWord_Prolog);
            });

        //ProcessString使用了
        //获取字符宽度
        batch_matcher.register_step("83 C0 E0 50 E8 ? ? ? ? D9 5C 24", 2, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(4), CFont::GetCharacterSizeNormalDispatch);
                injector::MakeCALL(addresses[1].i(4), CFont::GetCharacterSizeNormalDispatch);
            });

        //查找GetCharacterSizeNormal的引用获得
        batch_matcher.register_step("6A 01 57 E8 ? ? ? ? D9 5C 24 30", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(3), CFont::GetCharacterSizeDrawingDispatch);
            });

        //查找GetCharacterSizeNormal的引用获得
        batch_matcher.register_step("EB 12 6A 01 57 E8 ? ? ? ? D9 5C 24 24", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(5), CFont::GetCharacterSizeDrawingDispatch);
            });

        //RenderSingleBuffer使用了
        //绘制字符
        batch_matcher.register_step("F3 0F 11 0C 24 E8 ? ? ? ? 8B 35", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(5), CFont::PrintCharDispatch);
            });

        //另一个使用PrintChar的函数里面
        batch_matcher.register_step("E8 ? ? ? ? 6A 01 57 E8 ? ? ? ? D9 5C 24 30", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(), CFont::PrintCharDispatch);
            });

        //搜索"font3"
        //加载fonts.wtd中的font_chs
        batch_matcher.register_step("8B CE 50 E8 ? ? ? ? 80 3D ? ? ? ? 6A", 2, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(3), CFont::LoadTextureCB);
                injector::MakeCALL(addresses[1].i(3), CFont::LoadTextureCB);
            });

        //搜索"american.gxt"
        //改变GXT的文件名
        batch_matcher.register_step("80 7C 24 04 00 56 8B F1 74 07", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeJMP(addresses[0].i(), GetTextFileName);
            });

        //ProcessString使用了
        //GetStringWidth
        batch_matcher.register_step("B8 B4 10 00 00", 1, [](const std::vector<memory_pointer>& addresses)
            {
                injector::MakeJMP(addresses[0].i(-6), CFont::GetStringWidthRemake);
            });

        //使用了GetStringWidth
        //GetMaxWordWidth
        batch_matcher.register_step("51 56 8B 74 24 0C 85 F6 75 05 D9 EE 5E 59 C3 66", 1, [](const std::vector<memory_pointer>& addresses)
            {
                injector::MakeJMP(addresses[0].i(), CFont::GetMaxWordWidth);
            });
    }
}
