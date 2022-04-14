#include "common.h"

bool IsNativeChar(char32_t c)
{
    return c < 0x100;
}

bool IsNormalNativeChar(char32_t c)
{
    return IsNativeChar(c) && (c != 0) && (c != ' ') && (c != '~');
}

void SkipUTF8Signature(std::ifstream& stream)
{
    char bom[3];

    if (stream.get(bom[0]) && stream.get(bom[1]) && stream.get(bom[2]))
    {
        if (bom[0] == '\xEF' && bom[1] == '\xBB' && bom[2] == '\xBF')
        {
            return;
        }
    }

    stream.seekg(0);
}
