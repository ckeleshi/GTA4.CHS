#pragma once
#include "stdinc.h"

static constexpr uint g_CharsPerLine = 64;

typedef std::uint16_t GTAChar;

struct CVector2D
{
    float x;
    float y;
};

struct CRect
{
    CVector2D bottom_left;
    CVector2D top_right;
};

struct WhmTextData
{
    std::size_t hash;
    std::uintptr_t offset;
};
VALIDATE_SIZE(WhmTextData, 8);

bool IsNativeChar(char32_t c);
bool IsNormalNativeChar(char32_t c);
//读取流时跳过UTF8签名
void SkipUTF8Signature(std::ifstream& stream);
