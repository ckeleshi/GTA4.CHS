#pragma once
#include "../common/stdinc.h"
#include "../common/common.h"

namespace gta_string
{
    unsigned gtaWcslen(const GTAChar* str);

    //存档标题
    uchar* gtaTruncateString(uchar* dst, const GTAChar* src, unsigned size);
    void gtaExpandString(const uchar* src, GTAChar* dst);
    //显示存档覆盖提示时用于格式化的存档名称

    void gtaExpandString2(GTAChar* dst, const uchar* src, int a8);
    //TBoGT重玩任务选择界面
    void gtaExpandString3(GTAChar* dst, const uchar* src, int a8);

    //邮件/网页
    void gtaTruncateString2(const GTAChar* src, uchar* dst);
    void gtaExpandStringGxt(const uchar* src, GTAChar* dst);

    //接收宽字符串，但转换成utf8
    void __stdcall gtaMailAppendWideStringAsUtf8(int id, const GTAChar* str);

    uchar* gtaUTF8strncpy(uchar* dest, const uchar* source, unsigned size);
    void* gtaSpecialMemmove(uchar* dest, const uchar* source, unsigned size);

    unsigned gtaUTF8Strlen(const uchar* str);
}
