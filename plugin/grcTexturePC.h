#pragma once
#include "../common/stdinc.h"

struct grcTexturePC
{
    void *vtbl;       // datBase
    uint *m_pPageMap; // pgBase
    uchar m_nObjectType;
    uchar m_nbDepth;
    ushort m_nUsageCount;
    uint _fC;
    uint _f10;
    const char *m_pszName;
    IDirect3DTexture9 *m_piTexture;
    ushort m_wWidth;
    ushort m_wHeight;
    D3DFORMAT m_dwPixelFormat;
    ushort m_wStride;
    uchar m_eTextureType;
    uchar m_nbLevels;
    D3DVECTOR _f28;
    D3DVECTOR _f34;
    grcTexturePC *m_pPrev;
    grcTexturePC *m_pNext;
    uchar *m_pPixelData;
    uchar _f4C;
    uchar _f4D;
    uchar _f4E;
    uchar _f4F;

    grcTexturePC();
    ~grcTexturePC();

    void ReleaseTexture();
};

VALIDATE_SIZE(grcTexturePC, 0x50);
