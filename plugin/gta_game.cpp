#include "gta_game.h"
#include "plugin.h"

namespace gta_game
{
void register_patches(batch_matching &batch_matcher)
{
    // 搜索"~%c~"找到CFont::ProcessString

    // 使用"DD3D30"的代码下方
    batch_matcher.register_step("8B 3D ? ? ? ? 8B 68 04 8B 30", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.ppD3DDevice = *addresses[0].p<IDirect3DDevice9 **>(2);
    });

    // IDA Names: const rage::grcTexturePC::`vtable'
    batch_matcher.register_step("C7 45 00 ? ? ? ? C7 45 44 00 00 00 00", 1,
                                [](const byte_pattern::result_type &addresses) {
                                    plugin.game.game_addr.pTexturePCVirtualTable = *addresses[0].p<void *>(3);
                                });

    // https://github.com/ThirteenAG/GTAIV.EFLC.FusionFix/blob/master/source/dllmain.cpp
    // Native: GET_CURRENT_EPISODE(7D7619D2)里面
    // 11D6FD4
    batch_matcher.register_step("83 3D ? ? ? ? ? 8B 01 0F 44 C2 89 01 B0 01 C2 08 00", 2,
                                [](const byte_pattern::result_type &addresses) {
                                    plugin.game.game_addr.pGameEpisodeID = *addresses[0].p<int *>(2);
                                });

    // RenderSingleBuffer使用了
    batch_matcher.register_step("A1 ? ? ? ? 80 7C 24 08 00", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnGraphics_SetRenderState = addresses[0].p();
    });

    // GetCharacterSizeNormal使用了
    // ButtonWidths紧跟着Font_Datas
    batch_matcher.register_step("0F B6 84 08 ? ? ? ? 66 0F 6E C0", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.pFont_Datas = *addresses[0].p<CFontInfo *>(4);
        plugin.game.game_addr.pFont_ButtonWidths = reinterpret_cast<float *>(&plugin.game.game_addr.pFont_Datas[4]);
    });

    // GetStringWidth使用了
    batch_matcher.register_step("F3 0F 59 05 ? ? ? ? 88 4C 24 12", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.pFont_BlipWidth = *addresses[0].p<float *>(4);
    });

    // ProcessString使用了
    batch_matcher.register_step("8D AE ? ? ? ? 32 C0", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.pFont_Details = *addresses[0].p<CFontDetails *>(2);
    });

    // magic: AD 7F 33 31
    batch_matcher.register_step("81 3D ? ? ? ? AD 7F 33 31", 2, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.pFont_RenderState = *addresses[0].p<CFontRenderState *>(2);
    });

    // GetCharacterSizeNormal使用了
    batch_matcher.register_step("F3 0F 11 05 ? ? ? ? 66 0F 6E 84 24 AC 00 00", 1,
                                [](const byte_pattern::result_type &addresses) {
                                    plugin.game.game_addr.pFont_ResolutionX = *addresses[0].p<float *>(4);
                                });

    // ProcessString使用了
    batch_matcher.register_step("A1 ? ? ? ? 83 F8 FF 75 1E", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_GetRenderIndex = addresses[0].p();
    });

    // AD 7F 33 31所在的函数(RenderSingleBuffer)使用了
    batch_matcher.register_step("83 EC 30 83 3D ? ? ? ? FF", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_PrintChar = addresses[0].p();
    });

    // GetStringWidth使用了
    batch_matcher.register_step("51 55 56 E8", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_GetCharacterSizeNormal = addresses[0].p();
    });

    // PrintChar使用了
    batch_matcher.register_step("8A 0D ? ? ? ? 0F B6 D1", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_GetCharacterSizeDrawing = addresses[0].p();
    });

    // PrintChar使用了
    batch_matcher.register_step("83 EC 10 8B 44 24 14 F3 0F 7E 00", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_Render2DPrimitive = addresses[0].p();
    });

    //"font3"附近使用了
    batch_matcher.register_step("8B 54 24 08 53 56 8B 74 24 0C 80 3E 22", 1,
                                [](const byte_pattern::result_type &addresses) {
                                    plugin.game.game_addr.fnHash_HashStringFromSeediCase = addresses[0].p();
                                });

    //"font3"附近使用了
    batch_matcher.register_step("53 55 56 57 8B F9 85 FF 74 3F", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnDictionary_GetElementByKey = addresses[0].p();
    });

    // GetStringWidth使用了
    batch_matcher.register_step("83 C7 02 53 57 E8", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_ParseToken = injector::GetBranchDestination(addresses[0].p(5)).get();
    });

    // GetStringWidth使用了
    batch_matcher.register_step("83 EC 10 53 8B 5C 24 18 55 56", 1, [](const byte_pattern::result_type &addresses) {
        plugin.game.game_addr.fnFont_AddTokenStringWidth = addresses[0].p();
    });
}
} // namespace gta_game
