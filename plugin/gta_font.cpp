#include "gta_font.h"
#include "font.h"
#include "plugin.h"

namespace gta_font
{
// 922095
__declspec(naked) void ProcessToken_Epilog()
{
    __asm
    {
            pop ebp;
            call plugin.game.game_addr.fnFont_ProcessToken;
            or eax, 0x80000000;
            jmp ebp;
    }
}

// 922120
__declspec(naked) void ProcessToken_Epilog2()
{
    __asm
    {
            and ebp, 0x7FFFFFFF;
            movzx eax, word ptr[ebp - 4];
            sub ebp, 4;
            ret;
    }
}

// 922147
__declspec(naked) void ProcessToken_Epilog3()
{
    __asm
    {
            and ebp, 0x7FFFFFFF;
            xor eax, eax;
            test eax,eax;
            ret;
    }
}

void register_patches()
{
    // 搜索"~%c~"找到CFont::ProcessString

    // 储存CFont::ProcessString地址
    plugin.game.game_addr.fnFont_ProcessString = injector::aslr_ptr(0x88B370).get();

    // 劫持菜单项的CFont::ProcessString
    injector::MakeCALL(injector::aslr_ptr(0x88BC11).get(), CFont::ProcessStringRoutine);

    // ProcessToken epilog
    injector::MakeCALL(injector::aslr_ptr(0x88B613).get(), ProcessToken_Epilog);

    // ProcessString token branch2
    injector::MakeCALL(injector::aslr_ptr(0x88B687).get(), ProcessToken_Epilog2);
    injector::MakeNOP(injector::aslr_ptr(0x88B68C).get(), 2);

    // ProcessString token branch3
    injector::MakeCALL(injector::aslr_ptr(0x88B6A7).get(), ProcessToken_Epilog3);
    injector::MakeNOP(injector::aslr_ptr(0x88B6AC).get(), 2);

    // CFont::ProcessString使用了
    // 跳过单词
    injector::MakeJMP(injector::aslr_ptr(0x8859E0).get(), CFont::SkipWord_Prolog);

    // GetStringWidth使用了
    // 获取字符宽度
    injector::MakeCALL(injector::aslr_ptr(0x884B64).get(), CFont::GetCharacterSizeNormalDispatch);
    injector::MakeCALL(injector::aslr_ptr(0x88A7D5).get(), CFont::GetCharacterSizeNormalDispatch);

    // 查找GetCharacterSizeNormal的引用获得
    injector::MakeCALL(injector::aslr_ptr(0x884D0A).get(), CFont::GetCharacterSizeDrawingDispatch);

    // 查找GetCharacterSizeNormal的引用获得
    injector::MakeCALL(injector::aslr_ptr(0x88A58B).get(), CFont::GetCharacterSizeDrawingDispatch);

    // RenderSingleBuffer使用了
    // 绘制字符
    injector::MakeCALL(injector::aslr_ptr(0x88A4A0).get(), CFont::PrintCharDispatch);

    // 另一个使用PrintChar的函数里面
    injector::MakeCALL(injector::aslr_ptr(0x884D02).get(), CFont::PrintCharDispatch);

    // 加载fonts.wtd中的font_chs
    injector::MakeCALL(injector::aslr_ptr(0x887642).get(), CFont::LoadTextureCB);
    injector::MakeCALL(injector::aslr_ptr(0x887CB6).get(), CFont::LoadTextureCB);

    // ProcessString使用了
    // GetStringWidth
    injector::MakeJMP(injector::aslr_ptr(0x88A690).get(), CFont::GetStringWidthRemake);

    // 使用了GetStringWidth
    //  GetMaxWordWidth
    injector::MakeJMP(injector::aslr_ptr(0x88B2B0).get(), CFont::GetMaxWordWidth);
}
} // namespace gta_font
