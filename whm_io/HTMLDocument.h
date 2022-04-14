#pragma once
#include "../common/stdinc.h"
#include "pgBase.h"

enum HtmlNodeType :uint {
    Node_HtmlNode = 0,
    Node_HtmlDataNode = 1,
    Node_HtmlTableNode = 2,
    Node_HtmlTableElementNode = 3
};

enum HtmlTag :uint {
    HTMLTAG_HTML = 0,
    HTMLTAG_TITLE = 1,
    HTMLTAG_A = 2,
    HTMLTAG_BODY = 3,
    HTMLTAG_B = 4,
    HTMLTAG_BR = 5,
    HTMLTAG_CENTER = 6,
    HTMLTAG_CODE = 7,
    HTMLTAG_DL = 8,
    HTMLTAG_DT = 9,
    HTMLTAG_DD = 10,
    HTMLTAG_DIV = 11,
    HTMLTAG_EMBED = 12,
    HTMLTAG_EM = 13,
    HTMLTAG_HEAD = 14,
    HTMLTAG_H1 = 15,
    HTMLTAG_H2 = 16,
    HTMLTAG_H3 = 17,
    HTMLTAG_H4 = 18,
    HTMLTAG_H5 = 19,
    HTMLTAG_H6 = 20,
    HTMLTAG_IMG = 21,
    HTMLTAG_I = 22,
    HTMLTAG_LINK = 23,
    HTMLTAG_LI = 24,
    HTMLTAG_META = 25,
    HTMLTAG_OBJECT = 26,
    HTMLTAG_OL = 27,
    HTMLTAG_P = 28,
    HTMLTAG_PARAM = 29,
    HTMLTAG_SPAN = 30,
    HTMLTAG_STRONG = 31,
    HTMLTAG_STYLE = 32,
    HTMLTAG_TABLE = 33,
    HTMLTAG_TR = 34,
    HTMLTAG_TH = 35,
    HTMLTAG_TD = 36,
    HTMLTAG_UL = 37,
    HTMLTAG_TEXT = 38,
    HTMLTAG_SCRIPTOBJ = 39
};

enum HtmlAttrValue :uint {
    HTMLATTRVAL_LEFT = 0,
    HTMLATTRVAL_RIGHT = 1,
    HTMLATTRVAL_CENTER = 2,
    HTMLATTRVAL_JUSTIFY = 3,
    HTMLATTRVAL_TOP = 4,
    HTMLATTRVAL_BOTTOM = 5,
    HTMLATTRVAL_MIDDLE = 6,
    HTMLATTRVAL_INHERIT = 7,
    HTMLATTRVAL_XXSMALL = 8,
    HTMLATTRVAL_XSMALL = 9,
    HTMLATTRVAL_SMALL = 10,
    HTMLATTRVAL_MEDIUM = 11,
    HTMLATTRVAL_LARGE = 12,
    HTMLATTRVAL_XLARGE = 13,
    HTMLATTRVAL_XXLARGE = 14,
    HTMLATTRVAL_BLOCK = 15,
    HTMLATTRVAL_INLINE = 18,
    HTMLATTRVAL_NONE = 19,
    HTMLATTRVAL_SOLID = 20,
    HTMLATTRVAL_UNDERLINE = 21,
    HTMLATTRVAL_OVERLINE = 22,
    HTMLATTRVAL_LINETHROUGH = 23,
    HTMLATTRVAL_BLINK = 24,
    HTMLATTRVAL_REPEAT = 25,
    HTMLATTRVAL_NOREPEAT = 26,
    HTMLATTRVAL_REPEATX = 27,
    HTMLATTRVAL_REPEATY = 28,
    HTMLATTRVAL_COLLAPSE = 29,
    HTMLATTRVAL_SEPARATE = 30,
    HTMLATTRVAL_UNDEFINED = 0xFFFFFFFF
};

enum CssProperty :uint {
    CSS_WIDTH = 0,
    CSS_HEIGHT = 1,
    CSS_DISPLAY = 2,
    CSS_BACKGROUND_COLOR = 3,
    CSS_BACKGROUND_REPEAT = 4,
    CSS_BACKGROUND_POSITION = 5,
    CSS_BACKGROUND_IMAGE = 6,
    CSS_COLOR = 7,
    CSS_TEXT_ALIGN = 8,
    CSS_TEXT_DECORATION = 9,
    CSS_VERTICAL_ALIGN = 10,
    CSS_FONT = 11,
    CSS_FONT_SIZE = 12,
    CSS_FONT_STYLE = 13,
    CSS_FONT_WEIGHT = 14,
    CSS_BORDER_COLLAPSE = 15,
    CSS_BORDER_STYLE = 16,
    CSS_BORDER_BOTTOM_STYLE = 17,
    CSS_BORDER_LEFT_STYLE = 18,
    CSS_BORDER_RIGHT_STYLE = 19,
    CSS_BORDER_TOP_STYLE = 20,
    CSS_BORDER_COLOR = 21,
    CSS_BORDER_BOTTOM_COLOR = 22,
    CSS_BORDER_LEFT_COLOR = 23,
    CSS_BORDER_RIGHT_COLOR = 24,
    CSS_BORDER_TOP_COLOR = 25,
    CSS_BORDER_WIDTH = 26,
    CSS_BORDER_BOTTOM_WIDTH = 27,
    CSS_BORDER_LEFT_WIDTH = 28,
    CSS_BORDER_RIGHT_WIDTH = 29,
    CSS_BORDER_TOP_WIDTHT = 30,
    CSS_MARGIN_BOTTOM = 31,
    CSS_MARGIN_LEFT = 32,
    CSS_MARGIN_RIGHT = 33,
    CSS_MARGIN_TOP = 34,
    CSS_PADDING_BOTTOM = 35,
    CSS_PADDING_LEFT = 36,
    CSS_PADDING_RIGHT = 37,
    CSS_PADDING_TOP = 38,
    CSS_UNUSED = 39
};

struct HtmlRenderState {
    HtmlAttrValue       eDisplay;
    float               fWidth;
    float               fHeight;
    float               _fC;
    float               _f10;
    uchar               _f14[4];
    float               _f18;
    float               _f1C;
    uint                dwBgColor;
    pgPtr<void>         pBackgroundImage;
    uint                _f28h;
    uint                _f28l;
    HtmlAttrValue       backgroundRepeat;
    uint                dwColor;
    HtmlAttrValue       eAlign;
    HtmlAttrValue       eValign;
    HtmlAttrValue       eTextDecoration;
    uint                _f44;
    HtmlAttrValue       eFontSize;
    int                 nFontStyle;
    int                 nFontWeight;
    float               _f54;
    uint                dwBorderBottomColor;
    HtmlAttrValue       eBorderBottomStyle;
    float               fBorderBottomWidth;
    uint                dwBorderLeftColor;
    HtmlAttrValue       eBorderLeftStyle;
    float               dwBorderLeftWidth;
    uint                dwBorderRightColor;
    HtmlAttrValue       eBorderRightStyle;
    float               fBorderRightWidth;
    uint                dwBorderTopColor;
    HtmlAttrValue       eBorderTopStyle;
    float               fBorderTopWidth;
    float               fMarginBottom;
    float               fMarginLeft;
    float               fMarginRight;
    float               fMarginTop;
    float               fPaddingBottom;
    float               fPaddingLeft;
    float               fPaddingRight;
    float               fPaddingTop;
    float               fCellPadding;
    float               fCellSpacing;
    int                 nColSpan;
    int                 nRowSpan;
    bool                hasBackground;
    bool                isLink;
    uchar               _BA[2];
    uint                dwLinkColor;
    HtmlAttrValue       _fC0;
};

struct CHtmlCssDeclaration {
    CssProperty m_eProperty;
    uint       _f4; // data1
    uint       _f8; // data2
    uint       m_eDataType; // data type (0 - int, 1 - float, .., 3 - color, ..., 6 - unused)    
};

struct CHtmlCssSelector {
    HtmlTag                             m_eTag;
    pgObjectArray<CHtmlCssDeclaration>   m_aDeclarations;
    pgPtr<CHtmlCssSelector>              _fC;
};

struct CHtmlStylesheet {
    DWORD           _f0;    // unknown (hash?)
    pgObjectPtrArray<CHtmlCssSelector>   _f4;
    BYTE            _padC[3];
    BYTE            _fF;
    pgPtr<CHtmlStylesheet>   m_pNext;
};

struct CHtmlNode
{
    uint vtbl;
    HtmlNodeType m_eNodeType;
    pgPtr<CHtmlNode> m_pParentNode;
    pgObjectPtrArray<CHtmlNode> m_children;
    HtmlRenderState m_renderState;
};

struct CHtmlDataNode :CHtmlNode
{
    pgString m_pData;
};

struct CHtmlElementNode :CHtmlNode
{
    HtmlTag         m_eHtmlTag;
    pgString    m_pszTagName;
    pgObjectArray<char> m_nodeParam;
};

struct CHtmlTableNode :CHtmlElementNode
{
    pgPtr<void>   _fE8;
    pgPtr<void>   _fEC;
    pgPtr<void>   _fF0;
    pgPtr<void>   _fF4;
    pgPtr<void>   _fF8;
    DWORD   m_dwCellCount;
    DWORD   _f100;
};

struct CHtmlTableElementNode :CHtmlElementNode
{
    int    _fE8;
    int    _fEC;
};

struct CHtmlDocument
{
    pgPtr<CHtmlNode> m_pRootElement;
    pgPtr<CHtmlNode> m_pBody;
    pgString m_pszTitle;
    pgPtr<void> m_pTxd;
    pgObjectPtrArray<uint> _f10;
    pgObjectPtrArray<CHtmlNode> m_childNodes;
    pgObjectPtrArray<CHtmlStylesheet> m_pStylesheet;
    uchar pad[3];
    uchar _f2B;
};
