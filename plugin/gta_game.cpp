#include "gta_game.h"
#include "plugin.h"

namespace gta_game
{
    void register_patches(batch_matching &batch_matcher)
    {
        //搜索"~%c~"找到CFont::ProcessString

        // IDA Names: const rage::grcTexturePC::`vtable'
        batch_matcher.register_step("C7 45 00 ? ? ? ? C7 45 44 00 00 00 00", 1,
                                    [](const byte_pattern::result_type &addresses)
                                    {
                                        plugin.game.game_addr.pTexturePCVirtualTable = *addresses[0].p<void *>(3);
                                    });

        // RenderSingleBuffer使用了
        batch_matcher.register_step("A1 ? ? ? ? 80 7C 24 08 00", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnGraphics_SetRenderState = addresses[0].p(); });

        // GetCharacterSizeNormal使用了
        // ButtonWidths紧跟着Font_Datas
        batch_matcher.register_step("0F B6 84 08 ? ? ? ? 66 0F 6E C0", 1, [](const byte_pattern::result_type &addresses)
                                    {
        plugin.game.game_addr.pFont_Datas = *addresses[0].p<CFontInfo *>(4);
        plugin.game.game_addr.pFont_ButtonWidths = reinterpret_cast<float *>(&plugin.game.game_addr.pFont_Datas[4]); });

        // GetStringWidth使用了
        batch_matcher.register_step("F3 0F 59 05 ? ? ? ? 88 4C 24 12", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.pFont_BlipWidth = *addresses[0].p<float *>(4); });

        // ProcessString使用了
        batch_matcher.register_step("8D AE ? ? ? ? 32 C0", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.pFont_Details = *addresses[0].p<CFontDetails *>(2); });

        // magic: AD 7F 33 31
        batch_matcher.register_step("81 3D ? ? ? ? AD 7F 33 31", 2, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.pFont_RenderState = *addresses[0].p<CFontRenderState *>(2); });

        // GetCharacterSizeNormal使用了
        batch_matcher.register_step("F3 0F 11 05 ? ? ? ? 66 0F 6E 84 24 AC 00 00", 1,
                                    [](const byte_pattern::result_type &addresses)
                                    {
                                        plugin.game.game_addr.pFont_ResolutionX = *addresses[0].p<float *>(4);
                                    });

        // ProcessString使用了
        batch_matcher.register_step("A1 ? ? ? ? 83 F8 FF 75 1E", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_GetRenderIndex = addresses[0].p(); });

        // AD 7F 33 31所在的函数(RenderSingleBuffer)使用了
        batch_matcher.register_step("83 EC 30 83 3D ? ? ? ? FF", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_PrintChar = addresses[0].p(); });

        // GetStringWidth使用了
        batch_matcher.register_step("51 55 56 E8", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_GetCharacterSizeNormal = addresses[0].p(); });

        // PrintChar使用了
        batch_matcher.register_step("8A 0D ? ? ? ? 0F B6 D1", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_GetCharacterSizeDrawing = addresses[0].p(); });

        // PrintChar使用了
        batch_matcher.register_step("83 EC 10 8B 44 24 14 F3 0F 7E 00", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_Render2DPrimitive = addresses[0].p(); });

        // GetStringWidth使用了
        batch_matcher.register_step("83 C7 02 53 57 E8", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_ParseToken = injector::GetBranchDestination(addresses[0].p(5)).get(); });

        // GetStringWidth使用了
        batch_matcher.register_step("83 EC 10 53 8B 5C 24 18 55 56", 1, [](const byte_pattern::result_type &addresses)
                                    { plugin.game.game_addr.fnFont_AddTokenStringWidth = addresses[0].p(); });
    }
} // namespace gta_game
