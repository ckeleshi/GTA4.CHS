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
    plugin.game.game_addr.pGameEpisodeID = injector::aslr_ptr(0x112D754).get();

    // PrintChar使用了
    plugin.game.game_addr.fnGraphics_SetRenderState = injector::aslr_ptr(0x85CF60).get();

    // GetCharacterSizeNormal使用了
    // ButtonWidths紧跟着Font_Datas
    plugin.game.game_addr.pFont_Datas = injector::aslr_ptr(0x11EC930).get();
    plugin.game.game_addr.pFont_ButtonWidths = reinterpret_cast<float *>(&plugin.game.game_addr.pFont_Datas[4]);

    // GetStringWidth使用了
    plugin.game.game_addr.pFont_BlipWidth = injector::aslr_ptr(0x11EFB50).get();

    // ProcessString使用了
    plugin.game.game_addr.pFont_Details = injector::aslr_ptr(0x11F5BC8).get();

    // magic: AD 7F 33 31
    plugin.game.game_addr.pFont_RenderState = injector::aslr_ptr(0xF38114).get();

    // GetCharacterSizeNormal使用了
    plugin.game.game_addr.pFont_ResolutionX = injector::aslr_ptr(0xF3814C).get();

    // ProcessString使用了
    plugin.game.game_addr.fnFont_GetRenderIndex = injector::aslr_ptr(0x8840B0).get();

    // AD 7F 33 31所在的函数(RenderSingleBuffer)使用了
    plugin.game.game_addr.fnFont_PrintChar = injector::aslr_ptr(0x8843E0).get();

    // GetStringWidth使用了
    plugin.game.game_addr.fnFont_GetCharacterSizeNormal = injector::aslr_ptr(0x884110).get();

    // PrintChar使用了
    plugin.game.game_addr.fnFont_GetCharacterSizeDrawing = injector::aslr_ptr(0x874040).get();

    // PrintChar使用了
    plugin.game.game_addr.fnFont_Render2DPrimitive = injector::aslr_ptr(0x884300).get();

    //"font3"附近使用了
    plugin.game.game_addr.fnHash_HashStringFromSeediCase = injector::aslr_ptr(0x45CF50).get();

    // GetStringWidth使用了
    plugin.game.game_addr.fnFont_ParseToken = injector::aslr_ptr(0x887EF0).get();

    // ProcessString使用了
    plugin.game.game_addr.fnFont_ProcessToken = injector::aslr_ptr(0x889A10).get();

    //"font3"附近使用了
    plugin.game.game_addr.fnDictionary_GetElementByKey = injector::aslr_ptr(0x43A490).get();

    // GetStringWidth使用了
    plugin.game.game_addr.fnFont_AddTokenStringWidth = injector::aslr_ptr(0x884A90).get();
}
} // namespace gta_game
