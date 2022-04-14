#include "gta_phone.h"
#include "gta_string.h"

namespace gta_phone
{
    void register_patches(batch_matching& batch_matcher)
    {
        //用到"Unknown1"的地方往后一个调用的函数里面
        //TBoGT重玩任务选择界面
        batch_matcher.register_step("6A 64 53 57 E8", 2, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(4), gta_string::gtaExpandString3);
            });

        //Native: DISPLAY_TEXT_WITH_LITERAL_SUBSTRING(1FCB5241)里面第二个调用strncpy的地方
        batch_matcher.register_step("56 8D 44 24 10 53 50 E8", 1, [](const byte_pattern::result_type& addresses)
            {
                injector::MakeCALL(addresses[0].i(7), gta_string::gtaUTF8strncpy);
                injector::MakeNOP(addresses[0].i(37), 5);
            });
    }
}
