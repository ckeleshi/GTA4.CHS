#pragma once
#include "../common/common.h"
#include "../common/stdinc.h"


enum eTextAlignment
{
    ALIGN_CENTER = 0,    //居中对齐
    ALIGN_LEFT = 1,      //左对齐
    ALIGN_RIGHT = 2,     //右对齐
    ALIGN_LEFT_RIGHT = 3 //多余的宽度会平分到每个空格中
};

class CFontDetails
{
  public:
    uint field_0;
    float fScaleX;
    float fScaleY;
    float fBlipScaleX;
    int alignment;
    bool bDrawBox;
    bool field_15;
    bool bProportional;
    bool field_17;
    bool bUseUnderscore;
    bool bUseColor;
    bool field_1A;
    char pad1[1];
    uint BoxColor;
    float fWrapX;
    float fCentreWrapX;
    uchar nFont;
    uchar nExtraWidthIndex;
    char pad2[2];
    float fDropShadowSize;
    uint DropShadowColor;
    float fEdgeSize2;
    float fEdgeSize;
    float fLineHeight;
    float fExtraSpaceWidth;
    bool bIgnoreWidthLimit;
    char pad3[3];
};

VALIDATE_SIZE(CFontDetails, 0x48);

class CFontInfo
{
  public:
    uchar iPropValues[255];
    uchar iTextureMap[255];
    char pad1[2];
    float fUnpropValue;
    float fWidthOfSpaceBetweenChars[5];
    float fJapaneseSubFont1SpaceWidth;
    float fJapaneseSubFont2SpaceWidth;
    int iMainFontStart;
    int iMainFontEnd;
    int iSubFont1Start;
    int iSubFont1End;
    int iSubFont2Start;
    int iSubFont2End;
    int iCommonFontStart;
    int iCommonFontEnd;
    void *pTexture;
    float fTextureWidth;
    float fTextureHeight;
    float fSpriteSize;
    float field_250;
    float field_254;
};
VALIDATE_SIZE(CFontInfo, 0x258);

class CFontRenderState
{
  public:
    uint magic;
    float field_4;
    float field_8;
    float fScaleX;
    float fScaleY;
    float field_14;
    uint field_18;
    float field_1C;
    int field_20;
    bool field_24;
    uchar nExtraWidthIndex;
    bool bProportional;
    bool field_27;
    bool bUseUnderscore;
    uchar nFont;
    char pad1[2];
    float fEdgeSize;
    uint TokenType;
    bool field_34;
    char pad2[3];
};
VALIDATE_SIZE(CFontRenderState, 0x38);

class CFontBuffer
{
  public:
    CFontRenderState render_state;
    GTAChar buffer[996];
};
VALIDATE_SIZE(CFontBuffer, 2048);

struct TokenStruct
{
    int f0[4];
    GTAChar f10[4][32];

    union {
        int f110 = 0;
        uchar a110[4];
    };
};
VALIDATE_SIZE(TokenStruct, 0x114);

class CFont
{
  public:
    static void *__fastcall LoadTextureCB(void *, int, uint);

    static float GetCharacterSizeNormalDispatch(GTAChar chr);
    static float GetCHSCharacterSizeNormal();

    static float GetCharacterSizeDrawingDispatch(GTAChar chr, bool use_extra_width);
    static float GetCHSCharacterSizeDrawing(bool use_extra_width);

    static void PrintCharDispatch(float x, float y, GTAChar chr, bool buffered);
    static void PrintCHSChar(float x, float y, GTAChar chr);

    //判断字符是否为不能放在行首的标点符号
    static bool IsSpecialPunctuationMark(GTAChar chr);
    //将特殊字符看作单词的一部分
    static void SkipSpecialPunctuationMarks(const GTAChar *&str);
    static void AddSpecialPunctuationMarksWidth(const GTAChar *&str, float *width);

    static const GTAChar *SkipWord(const GTAChar *str);
    static const GTAChar *SkipWord_Prolog(std::uintptr_t address);
    static const GTAChar *SkipSpaces(const GTAChar *text);

    static float GetMaxWordWidth(const GTAChar *text);

    static float GetStringWidthRemake(const GTAChar *str, bool get_all);
};
