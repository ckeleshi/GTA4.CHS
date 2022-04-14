#pragma once
#include "batch_matching.h"

namespace gta_mail_reply
{
    struct string_storage
    {
        int f0;
        int f4;
        uchar* f8;
        int fC;
        int f10;
        int f14;
    };
    VALIDATE_SIZE(string_storage, 0x18);

    //4132E0的this指针
    struct class_for_mr
    {
        int f0;
        string_storage* f4;
        char f8[0x40];
        uchar f48[0x400];
        int f448; //0表示没有sso，转而读取f4
    };
    VALIDATE_SIZE(class_for_mr, 0x44C);

    void register_patches(batch_matching& batch_matcher);
}
