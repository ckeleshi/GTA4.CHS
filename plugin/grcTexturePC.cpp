#include "grcTexturePC.h"

grcTexturePC::grcTexturePC()
{
    vtbl = nullptr;
    m_pPageMap = nullptr;
    m_nObjectType = 0;
    m_nbDepth = 0;
    m_nUsageCount = 1;
    _fC = 0;
    _f10 = 0;
    m_pszName = "pack:/font_chs.dds";
    m_piTexture = nullptr;
    m_wWidth = 0;
    m_wHeight = 0;
    m_dwPixelFormat = D3DFMT_DXT5;
    m_wStride = 0;
    m_eTextureType = 0x80;
    m_nbLevels = 1;
    _f28 = D3DVECTOR{1.0f, 1.0f, 1.0f};
    _f34 = D3DVECTOR{0.0f, 0.0f, 0.0f};
    m_pPrev = nullptr;
    m_pNext = nullptr;
    m_pPixelData = nullptr;
    _f4C = 0;
    _f4D = 0;
    _f4E = 0;
    _f4F = 0;
}

grcTexturePC::~grcTexturePC()
{
    if (m_pPixelData)
    {
        delete[] m_pPixelData;
    }
}
