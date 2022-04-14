#include "gta_save.h"
#include "gta_string.h"

namespace gta_save
{
    void register_patches(batch_matching& batch_matcher)
    {
        //在使用"MO_SLOT"的函数里面
        batch_matcher.register_step("8D 84 24 4C 01 00 00 50 8D 44 24 1C 50 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(13), gta_string::gtaExpandString);
            });

        //存档覆盖提示
        batch_matcher.register_step("68 80 00 00 00 8D 44 24 50 50 68 ? ? ? ? E8", 2, [](const byte_pattern::result_type& addresses)
            {
                //使用"SG_AUTO_SL_OVR2"的地方往前一点
                injector::MakeCALL(addresses[0].i(15), gta_string::gtaExpandString2);

                //使用"SG_AUTO_SL_OVRWRT2"的地方往前一点
                injector::MakeCALL(addresses[1].i(15), gta_string::gtaExpandString2);
            });

        //使用"MO_BENCH_TEST"的函数里
        //加载黑屏右下角的任务名
        batch_matcher.register_step("FF 35 ? ? ? ? E8 ? ? ? ? 83 C4 0C 50 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(15), gta_string::gtaExpandString);
            });

        //在上方被替换掉的函数里面
        batch_matcher.register_step("6A 3C 05 ? ? ? ? 50 68 ? ? ? ? E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(13), gta_string::gtaTruncateString);
            });
    }
}
