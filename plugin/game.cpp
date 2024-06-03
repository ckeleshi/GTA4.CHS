#include "game.h"
#include "plugin.h"

void CGame::Graphics_SetRenderState(void *texture, int arg4)
{
    injector::cstd<void(void *, int)>::call(game_addr.fnGraphics_SetRenderState, texture, arg4);
}

void* CGame::Dictionary_grcTexturePC_GetElementByKey(void* dictionary, uint hash)
{
    return injector::thiscall<void* (void*, uint)>::call(game_addr.fnDictionary_GetElementByKey, dictionary, hash);
}

uint CGame::Hash_HashStringFromSeediCase(const char* str, uint hash)
{
    return injector::cstd<uint(const char*, uint)>::call(game_addr.fnHash_HashStringFromSeediCase, str, hash);
}

uchar CGame::Font_GetRenderIndex()
{
    return injector::cstd<uchar()>::call(game_addr.fnFont_GetRenderIndex);
}

float CGame::Font_GetCharacterSizeNormal(GTAChar chr)
{
    return injector::cstd<float(GTAChar)>::call(game_addr.fnFont_GetCharacterSizeNormal, chr);
}

float CGame::Font_GetCharacterSizeDrawing(GTAChar chr, bool use_extra_width)
{
    return injector::cstd<float(GTAChar, bool)>::call(game_addr.fnFont_GetCharacterSizeDrawing, chr, use_extra_width);
}

void CGame::Font_Render2DPrimitive(const CRect *screen_rect, const CRect *texture_rect, uint color, bool buffered)
{
    injector::cstd<void(const CRect *, const CRect *, uint, bool)>::call(game_addr.fnFont_Render2DPrimitive, screen_rect, texture_rect, color, buffered);
}

void CGame::Font_PrintChar(float posx, float posy, GTAChar chr, bool buffered)
{
    injector::cstd<void(float, float, GTAChar, bool)>::call(game_addr.fnFont_PrintChar, posx, posy, chr, buffered);
}

int CGame::Font_ParseToken(const GTAChar *text, GTAChar *text_to_show, TokenStruct *token_data)
{
    return injector::cstd<int(const GTAChar *, GTAChar *, TokenStruct *)>::call(
        game_addr.fnFont_ParseToken, text, text_to_show, token_data);
}

const GTAChar *CGame::Font_ProcessToken(const GTAChar *text, uint *color, bool get_color, uchar *color_code,
                                        int *key_number, bool *is_new_line_token, GTAChar *text_to_show, TokenStruct *token_data)
{
    return injector::cstd<const GTAChar *(const GTAChar *, uint *, bool, uchar *, int *, bool *, GTAChar *, TokenStruct *)>::call(
        game_addr.fnFont_ProcessToken, text, color, get_color, color_code, key_number, is_new_line_token,
        text_to_show, token_data);
}

void CGame::Font_AddTokenStringWidth(const GTAChar *text, float *width, int render_index)
{
    return injector::cstd<void(const GTAChar *, float *, int)>::call(game_addr.fnFont_AddTokenStringWidth, text, width, render_index);
}

void CGame::MailAppendByteString(int id, const uchar *str)
{
    injector::stdcall<void(int, const uchar *)>::call(game_addr.fnMailAppendByteString, id, str);
}
