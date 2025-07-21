#include "gta_game.h"
#include "plugin.h"

namespace gta_game
{
void register_patches()
{
    // 搜索"~%c~"找到CFont::ProcessString

    // https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/master/source/dllmain.cpp
    // Native: GET_CURRENT_EPISODE(7D7619D2)里面
    // 11D6FD4
    plugin.game.game_addr.pGameEpisodeID = injector::aslr_ptr(0x10F8044).get();

    // PrintChar使用了
    plugin.game.game_addr.fnGraphics_SetRenderState = injector::aslr_ptr(0x8524C0).get();

    // GetCharacterSizeNormal使用了
    // ButtonWidths紧跟着Font_Datas
    plugin.game.game_addr.pFont_Datas = injector::aslr_ptr(0x1176B58).get();
    plugin.game.game_addr.pFont_ButtonWidths = reinterpret_cast<float *>(&plugin.game.game_addr.pFont_Datas[4]);

    // GetStringWidth使用了
    plugin.game.game_addr.pFont_BlipWidth = injector::aslr_ptr(0x1179D78).get();

    // ProcessString使用了
    plugin.game.game_addr.pFont_Details = injector::aslr_ptr(0x117FDF0).get();

    // magic: AD 7F 33 31
    plugin.game.game_addr.pFont_RenderState = injector::aslr_ptr(0xF1531C).get();

    // GetCharacterSizeNormal使用了
    plugin.game.game_addr.pFont_ResolutionX = injector::aslr_ptr(0xF15354).get();

    // ProcessString使用了
    plugin.game.game_addr.fnFont_GetRenderIndex = injector::aslr_ptr(0x7F7380).get();

    // AD 7F 33 31所在的函数(RenderSingleBuffer)使用了
    plugin.game.game_addr.fnFont_PrintChar = injector::aslr_ptr(0x7F76B0).get();

    // GetStringWidth使用了
    plugin.game.game_addr.fnFont_GetCharacterSizeNormal = injector::aslr_ptr(0x7F73E0).get();

    // PrintChar使用了
    plugin.game.game_addr.fnFont_GetCharacterSizeDrawing = injector::aslr_ptr(0x7E72F0).get();

    // PrintChar使用了
    plugin.game.game_addr.fnFont_Render2DPrimitive = injector::aslr_ptr(0x7F75D0).get();

    //"font3"附近使用了
    plugin.game.game_addr.fnHash_HashStringFromSeediCase = injector::aslr_ptr(0x5B1C30).get();

    // GetStringWidth使用了
    plugin.game.game_addr.fnFont_ParseToken = injector::aslr_ptr(0x7FB1C0).get();

    // GetStringWidth使用了
    plugin.game.game_addr.fnFont_AddTokenStringWidth = injector::aslr_ptr(0x7F7D40).get();
}
} // namespace gta_game
